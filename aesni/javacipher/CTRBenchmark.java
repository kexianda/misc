import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Calendar;
import java.util.Random;
import java.util.TimeZone;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;


import javax.crypto.spec.SecretKeySpec;

/**
 * Usage: java -XX:+UseAESCTRIntrinsics CTRBenchMark
 */
public class CTRBenchmark {

    //private static final int KEY_LENGTH = 128;
    private static final int BLOCK_SIZE = 16;
    private static final String ALGORITHM = "AES";
    private static final String MODE = "CTR";
    //private static final String PROVIDER = "SunJCE";

    private static int CHUNK_SIZE = 1024 * 1024 * 64; // 
    private static int TOTAL_SIZE = 1024 * 1024 * 1024;// totol: 
    private static int KEY_SIZE = 16; // bytes

    public static void main(String[] args) {
        warnUpThreshold();
        testThroughput();
    }

    public static void testThroughput() {
        int chunkSizes[] = {1024 * 512, 1024 * 1024, 1024 * 1024 * 64};
        int[] keySizes = {16, 24, 32};
        for (int k = 0; k < keySizes.length; k++) {
            KEY_SIZE = keySizes[k];
            for (int i = 0; i < chunkSizes.length; ++i) {
                CHUNK_SIZE = chunkSizes[i];
                System.out.print("Total=" + ((float) TOTAL_SIZE) / (1024 * 1024)
                        + "Mb. Chunk=" + ((float) CHUNK_SIZE) / (1024 * 1024) + "Mb. "
                        + "Key=" + KEY_SIZE * 8 + "bits. ");
                testEncryption();
                System.out.print("Total=" + ((float) TOTAL_SIZE) / (1024 * 1024)
                        + "Mb. Chunk=" + ((float) CHUNK_SIZE) / (1024 * 1024) + "Mb. "
                        + "Key=" + KEY_SIZE * 8 + "bits. ");
                testDecryption();
            }
        }
    }

    public static void testEncryption() {
        double seconds = 0.0;
        int itrNum = 10;
        for(int i = 0; i < itrNum; i++) {
            seconds += TimeOfOneRound(true);
        } 
        double throughput = (TOTAL_SIZE / (1024 * 1204)) / (seconds/10);
        printResult("CTR encryption throughput:\t", throughput);
        
        
        //printResult("AES-CTR encryption throughput:\t", throughput);
//        try {
//            byte[] keyBytes = new byte[KEY_SIZE];
//            //128 bit counter
//            byte[] ivBytes = new byte[BLOCK_SIZE];
//
//            Random r = new Random();
//            r.nextBytes(ivBytes);
//            r.nextBytes(keyBytes);
//
//            SecretKeySpec key = new SecretKeySpec(keyBytes, ALGORITHM);
//            IvParameterSpec ivSpec = new IvParameterSpec(ivBytes);
//            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");
//
//            //encryption
//            //-XX:CompileThreshold=10
//            // 100 > CompileThreshold
//            //------------------------------------------
//            //encryption
//            byte[][] encInputChunks = prepareTwoDimensionsData(TOTAL_SIZE / CHUNK_SIZE, CHUNK_SIZE);
//            cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec);
//
//            long begin = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
//            for (int i = 0; i < TOTAL_SIZE / CHUNK_SIZE; i++) {
//                byte[] encOutput = cipher.doFinal(encInputChunks[i]);
//            }
//            long end = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
//            double seconds = (double) (end - begin) / 1000;
//            double throughput = (TOTAL_SIZE / (1024 * 1204)) / seconds;
//            printResult("AES-CTR encryption throughput:\t", throughput);
//
//        } catch (Exception e) {
//            System.out.println(e.getMessage());
//        }

    }

    public static void testDecryption() {
        double seconds = 0.0;
        int itrNum = 10;
        for (int i = 0; i < itrNum; i++) {
            seconds += TimeOfOneRound(false);
        }
        double throughput = (TOTAL_SIZE / (1024 * 1204)) / (seconds / 10);
        printResult("CTR decryption throughput:\t", throughput);
//
//        try {
//            //128 key
//            byte[] keyBytes = new byte[KEY_SIZE];
//            //128 bit counter
//            byte[] ivBytes = new byte[BLOCK_SIZE];
//
//            Random r = new Random();
//            r.nextBytes(ivBytes);
//            r.nextBytes(keyBytes);
//
//            SecretKeySpec key = new SecretKeySpec(keyBytes, ALGORITHM);
//            IvParameterSpec ivSpec = new IvParameterSpec(ivBytes);
//            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");
//
//            //encryption
//            //-XX:CompileThreshold=10
//            // 100 > CompileThreshold
//            //------------------------------------------
//            //encryption
//            //------------------------------------------
//            //decryption
//            //warnUpThreshold();
//            byte[][] decInputChunks = prepareTwoDimensionsData(TOTAL_SIZE / CHUNK_SIZE, CHUNK_SIZE);
//            cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);
//
//            long begin = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
//            for (int i = 0; i < TOTAL_SIZE / CHUNK_SIZE; i++) {
//                byte[] decOutput = cipher.doFinal(decInputChunks[i]);
//            }
//            long end = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
//            double seconds = (double) (end - begin) / 1000;
//            double throughput = (TOTAL_SIZE / (1024 * 1204)) / seconds; // MB/s
//            printResult("AES-CTR decryption throughput:\t", throughput);
//
//        } catch (Exception e) {
//            System.out.println(e.getMessage());
//        }

    }
    
    private static double TimeOfOneRound(boolean isEncrypt){
        double seconds = 0.0;
        try {
            byte[] keyBytes = new byte[KEY_SIZE];
            //128 bit counter
            byte[] ivBytes = new byte[BLOCK_SIZE];

            Random r = new Random();
            r.nextBytes(ivBytes);
            r.nextBytes(keyBytes);

            SecretKeySpec key = new SecretKeySpec(keyBytes, ALGORITHM);
            IvParameterSpec ivSpec = new IvParameterSpec(ivBytes);
            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");

            //encryption
            //-XX:CompileThreshold=10
            // 100 > CompileThreshold
            //------------------------------------------
            //encryption
            byte[][] encInputChunks = prepareTwoDimensionsData(TOTAL_SIZE / CHUNK_SIZE, CHUNK_SIZE);
            cipher.init(isEncrypt ? Cipher.ENCRYPT_MODE : Cipher.DECRYPT_MODE, key, ivSpec);

            long begin = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
            for (int i = 0; i < TOTAL_SIZE / CHUNK_SIZE; i++) {
                byte[] encOutput = cipher.doFinal(encInputChunks[i]);
            }
            long end = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
            seconds = (double) (end - begin) / 1000;

        } catch (Exception e) {
            System.out.println(e.getMessage());
        } 
        
        return seconds;
    }
    
    public static void warnUpThreshold() {

        try {
            //128 key
            byte[] keyBytes = new byte[BLOCK_SIZE];
            //128 bit counter
            byte[] ivBytes = new byte[BLOCK_SIZE];

            Random r = new Random();
            r.nextBytes(ivBytes);
            r.nextBytes(keyBytes);

            SecretKeySpec key = new SecretKeySpec(keyBytes, ALGORITHM);
            IvParameterSpec ivSpec = new IvParameterSpec(ivBytes);
            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");

            //encryption
            //-XX:CompileThreshold=10000
            // totalSize/chunkSize > 10000 default threshold
            //------------------------------------------
            int totalSize = 1024 * 1024 * 1024; //
            int chunkSize = 512;
            //encryption
            byte[][] encInputChunks = prepareTwoDimensionsData(totalSize / chunkSize, chunkSize);
            cipher.init(Cipher.ENCRYPT_MODE, key, ivSpec);
            for (int i = 0; i < (totalSize / chunkSize); i++) {
                byte[] encOutput = cipher.doFinal(encInputChunks[i]);
            }
            System.out.println("CTR encryption is warmed up!");

            //------------------------------------------
            //decryption
            byte[][] decInputChunks = prepareTwoDimensionsData(totalSize / chunkSize, chunkSize);
            cipher.init(Cipher.DECRYPT_MODE, key, ivSpec);
            for (int i = 0; i < (totalSize / chunkSize); i++) {
                byte[] decOutput = cipher.doFinal(decInputChunks[i]);
            }
            System.out.println("CTR decryption is warmed up!");
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        
        for(int i=0; i<10;i++) {
            TimeOfOneRound(true);
            TimeOfOneRound(false);
        }
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

    private static void printResult(String hints, double throughput) {
        System.out.print(hints);
        NumberFormat formatter = new DecimalFormat("#0.00");
        System.out.println(formatter.format(throughput) + " MB/s.");
    }
}
