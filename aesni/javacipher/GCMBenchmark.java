
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Calendar;
import java.util.Random;
import java.util.TimeZone;
import javax.crypto.Cipher;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * Usage: java -XX:+UseCTRAESIntrinsics CTRBenchMark
 */
public class GCMBenchmark {

    private static final int BLOCK_SIZE = 16;
    private static final int GCM_NONCE_LENGTH = 12; // recommented length: 12 * 8 = 96
    private static final int GCM_TAG_LENGTH = 16; // bytes
    private static final String ALGORITHM = "AES";
    private static final String MODE = "GCM";
    
    private static String PROVIDER = "SunJCE";  // DC

    private static int CHUNK_SIZE = 1024 * 1024 * 64; // 64M
    private static int TOTAL_SIZE = 1024 * 1024 * 1024;//total: 
    private static int KEY_SIZE = 16;//16*8=128(default), 24*8=192, 32*8=256

    public static void main(String[] args) {
        warmUpJvmThreshold();
        warmUpCpuFrequncy();
        
        if(args.length >= 1) {
            PROVIDER = args[0];
        }
        System.out.println("PROVIDER: " + PROVIDER);
        
        testGCMWithDifferentDataSizeKeyLength();
    }
    
    private static void testGCMWithDifferentDataSizeKeyLength() {
        //KEY_SIZE = 16; 24, 32  bytes*8 = bits
        int[] keySizeArr = {16, 24, 32};
        int totalSize = 1024 * 1024 * 8;
        int[] chunkSizeArr = {
            //64 * 1024,
            1024 * 1024,
            1024 * 1024 * 2,
            1024 * 1024 * 4
        };

        for (int i = 0; i < chunkSizeArr.length; i++) {
            for (int j = 0; j < 3; j++) {
                testGCMThroughput(keySizeArr[j], totalSize, chunkSizeArr[i], true);
                testGCMThroughput(keySizeArr[j], totalSize, chunkSizeArr[i], false);
            }
        }
    }
    
    private static void testGCMThroughput(int keySize, int totalSize, int chunkSize, boolean isEncrypt) {

        long mseconds = 0;
        int itrNum = 5;

        for (int i = 0; i < itrNum; i++) {
            mseconds += TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, isEncrypt);
        }

        NumberFormat formatter = new DecimalFormat("#0.00");
        double totalM = (double) (totalSize / (1024 * 1024));
        double chunkU = chunkSize >= (1024 * 1024) ? (double) chunkSize / (1024 * 1024) : (double) chunkSize / 1024;
        String chunkUnit = chunkSize >= (1024 * 1024) ? "MB" : "KB";
        double throughput = itrNum * totalM / ((double) mseconds / 1000); // MB/s
        System.out.println(
                MODE + (isEncrypt ? "-CTR Encryption, " : "-CTR Decryption, ")
                + "Total=" + formatter.format(totalM) + "MB, "
                + (isEncrypt ? ("Chunk=" + formatter.format(chunkU) + chunkUnit + ", ") : "\t\t")
                + "Key=" + (keySize * 8) + "bits"
                + " :\t"
                + formatter.format(throughput) + " MB/s."
        );
    }

    private static long TimeOfRunAesGCMOneRound(int keySize, int totalSize, int chunkSize, boolean isEncrypt) {
        long mseconds = 0;
        try {
            byte[] keyBytes = new byte[keySize];
            byte[] nonceBytes = new byte[GCM_NONCE_LENGTH];

            Random r = new Random();
            r.nextBytes(nonceBytes);
            r.nextBytes(keyBytes);
            byte[] aad = "I'm Ke, Xianda".getBytes();

            SecretKeySpec key = new SecretKeySpec(keyBytes, ALGORITHM);
            GCMParameterSpec spec = new GCMParameterSpec(GCM_TAG_LENGTH * 8, nonceBytes);
            
            if(isEncrypt) {
                Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding", PROVIDER);
                cipher.init(Cipher.ENCRYPT_MODE, key, spec);
                cipher.updateAAD(aad);

                byte[][] inputChunks = prepareTwoDimensionsData(totalSize / chunkSize, chunkSize);
                byte[] output = new byte[totalSize + BLOCK_SIZE];
                //---------------------------------------------------------------------------------
                long begin = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();

                for (int i = 0; i < totalSize / chunkSize; i++) {
                    //byte[] output = cipher.doFinal(inputChunks[i]);
                    if(i != (totalSize / chunkSize -1)) {
                        cipher.update(inputChunks[i], 0, chunkSize, output, i * chunkSize);
                    }
                    else {
                        cipher.doFinal(inputChunks[i], 0, chunkSize, output, i * chunkSize);
                    }
                }
                long end = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
                //---------------------------------------------------------------------------------   
                mseconds = end - begin;
            }
            else {
                //encrypt plain data.
                Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding", PROVIDER);
                cipher.init(Cipher.ENCRYPT_MODE, key, spec);
                cipher.updateAAD(aad.clone());
                byte[][] inputChunks = prepareTwoDimensionsData(totalSize / chunkSize, chunkSize);
                byte[] cipherText = new byte[totalSize + BLOCK_SIZE];
                for (int i = 0; i < totalSize / chunkSize; i++) {
                    if (i != (totalSize / chunkSize - 1)) {
                        cipher.update(inputChunks[i], 0, chunkSize, cipherText, i * chunkSize);
                    } else {
                        cipher.doFinal(inputChunks[i], 0, chunkSize, cipherText, i * chunkSize);
                    }
                }
                
                //decrypt cipher data
                Cipher deCipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding", PROVIDER);
                deCipher.init(Cipher.DECRYPT_MODE, key, spec);
                deCipher.updateAAD(aad);
                //byte[] plainText = new byte[totalSize];
                
                long begin = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
                deCipher.doFinal(cipherText);
//                for (int i = 0; i < totalSize / chunkSize; i++) {
//                    if (i != (totalSize / chunkSize - 1)) {
//                        deCipher.update(cipherText, i * chunkSize, chunkSize, plainText, i * chunkSize);
//                    } else {
//                        deCipher.doFinal(cipherText, i * chunkSize, chunkSize + BLOCK_SIZE, plainText, 0);
//                    }
//                }
                long end = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
                mseconds = end - begin;
            }

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }

        return mseconds;
    }

    //CPU intensive
    private static void warmUpCpuFrequncy() {
        int keySize = 16;
        int totalSize = 1024 * 1024;
        int chunkSize = 1024;

        keySize = 16;
        TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, false);

        keySize = 24;
        TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, false);

        keySize = 32;
        TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, false);

        System.out.println("CPU & JVM is warmed-up.");
    }

    public static void warmUpJvmThreshold() {
        int keySize = 16;
        int totalSize = 128;
        int chunkSize = 32;
        
        for(int i =0; i<200000; i++) {
            TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, true);
            TimeOfRunAesGCMOneRound(keySize, totalSize, chunkSize, false);
        }

        System.out.println("Warm up JVM threshold...");
    }

    protected static byte[][] prepareTwoDimensionsData(int strideNum, int stride) {
        byte[] dataEntry = new byte[stride];
        byte[][] result = new byte[strideNum][stride];
        Random r = new Random();
        r.nextBytes(dataEntry);
        for (int i = 0; i < strideNum; i++) {
            result[i] = dataEntry.clone();
        }
        return result;
    }

}
