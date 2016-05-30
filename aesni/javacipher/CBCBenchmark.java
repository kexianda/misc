
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Calendar;
import java.util.Random;
import java.util.TimeZone;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * Usage: java -XX:+UseCTRAESIntrinsics CTRBenchMark
 */
public class CBCBenchmark {

    private static final int BLOCK_SIZE = 16;
    private static final String ALGORITHM = "AES";
    private static final String MODE = "CBC";
    //private static final String PROVIDER = "SunJCE";

    private static int CHUNK_SIZE = 1024 * 1024 * 64; // 64M
    private static int TOTAL_SIZE = 1024 * 1024 * 1024;//total: 
    private static int KEY_SIZE = 16;//16*8=128(default), 24*8=192, 32*8=256

    public static void main(String[] args) {
        warmUpJvmThreshold();
        warmUpCpuFrequncy();

        //KEY_SIZE = 16; 24, 32  bytes*8 = bits
        int[] keySizeArr = {16, 24, 32};
        int totalSize = 1024 * 1024 * 1024;
        int[] chunkSizeArr = {
            64 * 1024,
            1024 * 1024,
            1024 * 1024 * 64
        };
        boolean isEncrypt = false;
        for (int i = 0; i < chunkSizeArr.length; i++) {
            for (int j = 0; j < 3; j++) {
                testCBCThroughput(keySizeArr[j], totalSize, chunkSizeArr[i], false);
            }
        }
    }

    private static void testCBCThroughput(int keySize, int totalSize, int chunkSize, boolean isEncrypt) {

        long mseconds = 0;
        int itrNum = 20;

        for (int i = 0; i < itrNum; i++) {
            mseconds += TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, isEncrypt);
        }

        NumberFormat formatter = new DecimalFormat("#0.00");
        double totalM = (double) (totalSize / (1024 * 1024));
        double chunkU = chunkSize >= (1024 * 1024) ? (double) chunkSize / (1024 * 1024) : (double) chunkSize / 1024;
        String chunkUnit = chunkSize >= (1024 * 1024) ? "MB" : "KB";
        double throughput = itrNum * totalM / ((double) mseconds / 1000); // MB/s
        System.out.println(
                (isEncrypt ? "Encryption, " : "Decryption, ")
                + "Total=" + formatter.format(totalM) + "MB, "
                + "Chunk=" + formatter.format(chunkU) + chunkUnit + ", "
                + "Key=" + (keySize * 8) + "bits"
                + " :\t"
                + formatter.format(throughput) + " MB/s."
        );
    }

    private static long TimeOfRunAesCBCOneRound(int keySize, int totalSize, int chunkSize, boolean isEncrypt) {
        long mseconds = 0;
        try {
            byte[] keyBytes = new byte[keySize];

            //128 bit iv
            byte[] ivBytes = new byte[BLOCK_SIZE];

            Random r = new Random();
            r.nextBytes(ivBytes);
            r.nextBytes(keyBytes);

            SecretKeySpec key = new SecretKeySpec(keyBytes, ALGORITHM);
            IvParameterSpec ivSpec = new IvParameterSpec(ivBytes);
            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");

            byte[][] decInputChunks = prepareTwoDimensionsData(totalSize / chunkSize, chunkSize);
            cipher.init(isEncrypt ? Cipher.ENCRYPT_MODE : Cipher.DECRYPT_MODE, key, ivSpec);

            long begin = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();

            for (int i = 0; i < totalSize / chunkSize; i++) {
                byte[] decOutput = cipher.doFinal(decInputChunks[i]);
            }

            long end = Calendar.getInstance(TimeZone.getTimeZone("UTC")).getTimeInMillis();
            mseconds = end - begin;

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }

        return mseconds;
    }

    //CPU intensive
    private static void warmUpCpuFrequncy() {
        int keySize = 16;
        int totalSize = 1024 * 1024 * 1024;
        int chunkSize = 1024 * 1024;

        keySize = 16;
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, false);

        keySize = 24;
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, false);

        keySize = 32;
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, false);

        System.out.println("Warm up CPU...");
    }

    public static void warmUpJvmThreshold() {
        int keySize = 16;
        int totalSize = 1024 * 1024 * 64;
        int chunkSize = 512;

        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, true);
        TimeOfRunAesCBCOneRound(keySize, totalSize, chunkSize, false);

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
