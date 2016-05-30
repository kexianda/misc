import java.util.Arrays;
import javax.crypto.*;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class TestGCM {

    private static final int BLOCK_SIZE = 16;
    //private static final int GCM_NONCE_LENGTH = 12; // recommented length: 12 * 8 = 96
    private static final int GCM_TAG_LENGTH = 16; // bytes
    private static final String ALGORITHM = "AES";
    private static final String MODE = "GCM";
    //private static final String PROVIDER = "SunJCE";


    private static int KEY_SIZE = 16;//16*8=128(default), 24*8=192, 32*8=256

    public static void main(String[] args) throws Exception {

        //nist_gcm_case_sanity();

        // NIST GCM test vectors
//        nist_gcm_case1();
//        nist_gcm_case2();
//        nist_gcm_case3();
//        nist_gcm_case4();
          nist_gcm_case5();
//        nist_gcm_case6();
//        nist_gcm_case7();
//        nist_gcm_case8();
//        nist_gcm_case9();
    }

    public static int nist_gcm_case_sanity() {
        int errNo = 0;
        String k_hex = "feffe9928665731c6d6a8f9467308308";
        String p_hex = "d9313225f88406e5a55909c5aff5269a"
                + "86a7a9531534ffda2e4c303d8a318aee"
                + "aa3c0c95956809ff2fcf0e2449a6ee25"
                + "b1aaedf5aa0de657ff637b391aeed255"
                + "d931aa25f88406e5a5ff09c5eef5269a"
                + "86a7a9aa1534f7da2e4cffee8a318a72"
                + "1c3c0c95aa6809532fcfeeff49a6b525"
                + "b16aedf5aaaae657baee7b39ffafd255"
                + "ffffffff";
        String iv_hex = "cafebabefffffffffffffffe"; // 12 bytes = 96bits
        String aad_hex = "feedfacedeadbeeffeedfacedeadbeef";

        //expected result
        String c_hex = "0633064f14e4556179c84fecfa8bffd7"
                + "ab48a21791bdd72e009b0a0ba8f6b8e2"
                + "9fce48084817d0a33eafeb5f311e3605"
                + "71b59ac4f4e22b4a68de46eb49cdff22"
                + "11028f41af5215e690dd44ca5055ff0e"
                + "38ddcb6c18ca153a9a1fc03fa56604fe"
                + "8dc52dc1bbf8ce95eff3dd32b7d98cbb"
                + "e34f5c6a40024554bd30a6bb3d565c24"
                + "1432cf28";
        String t_hex = "b0598aef7162a5f1caeda4e02842d5c2";

        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "sanity test");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "sanity test");

        return errNo;
    }

    //http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/gcm/gcm-spec.pdf
    public static int nist_gcm_case1() {
        String k_hex = "00000000000000000000000000000000";
        String p_hex = "";
        String iv_hex = "000000000000000000000000"; // 12 bytes = 96bits
        String aad_hex = "";

        //expected result
        String c_hex = "";
        String t_hex = "58e2fccefa7e3061367f1d57a4e7455a";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase1");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase1");
        return errNo;
    }

    public static int nist_gcm_case2() {
        String k_hex = "00000000000000000000000000000000";
        String p_hex = "00000000000000000000000000000000";
        String iv_hex = "000000000000000000000000"; // 12 bytes = 96bits
        String aad_hex = "";

        //expected result
        String c_hex = "0388dace60b6a392f328c2b971b2fe78";
        String t_hex = "ab6e47d42cec13bdf53a67b21257bddf";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase2");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase2");
        return errNo;
    }

    public static int nist_gcm_case3() {
        // without AAD

        String k_hex = "feffe9928665731c6d6a8f9467308308";
        String p_hex = "d9313225f88406e5a55909c5aff5269a"
                + "86a7a9531534f7da2e4c303d8a318a72"
                + "1c3c0c95956809532fcf0e2449a6b525"
                + "b16aedf5aa0de657ba637b391aafd255";
        String iv_hex = "cafebabefacedbaddecaf888"; // 12 bytes = 96bits
        String aad_hex = "";

        //expected result
        String c_hex = "42831ec2217774244b7221b784d0d49c"
                + "e3aa212f2c02a4e035c17e2329aca12e"
                + "21d514b25466931c7d8f6a5aac84aa05"
                + "1ba30b396a0aac973d58e091473f5985";
        String t_hex = "4d5c2af327cd64a62cf35abd2ba6fab4";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase3");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase3");
        return errNo;
    }

    public static int nist_gcm_case4() {
        String k_hex = "feffe9928665731c6d6a8f9467308308";
        String p_hex = "d9313225f88406e5a55909c5aff5269a"
                + "86a7a9531534f7da2e4c303d8a318a72"
                + "1c3c0c95956809532fcf0e2449a6b525"
                + "b16aedf5aa0de657ba637b39";
        String iv_hex = "cafebabefacedbaddecaf888"; // 12 bytes = 96bits
        String aad_hex = "feedfacedeadbeeffeedfacedeadbeef"
                + "abaddad2";

        //expected result
        String c_hex = "42831ec2217774244b7221b784d0d49c"
                + "e3aa212f2c02a4e035c17e2329aca12e"
                + "21d514b25466931c7d8f6a5aac84aa05"
                + "1ba30b396a0aac973d58e091";
        String t_hex = "5bc94fbc3221a5db94fae95ae7121a47";

        int errNo = 0;
        //errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase4");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase4");
        return errNo;
    }

    public static int nist_gcm_case5() {
        // iv 8 bytes < 96/8 bytes
        // add: 20 bytes > 16 bytes
        // p:  partial block

        String k_hex = "feffe9928665731c6d6a8f9467308308";

        String p_hex = "d9313225f88406e5a55909c5aff5269a"
                + "86a7a9531534f7da2e4c303d8a318a72"
                + "1c3c0c95956809532fcf0e2449a6b525"
                + "b16aedf5aa0de657ba637b39";

        String iv_hex = "cafebabefacedbad"; // 64bits < 96bits

        String aad_hex = "feedfacedeadbeeffeedfacedeadbeef"
                + "abaddad2";
        //expected result
        String c_hex = "61353b4c2806934a777ff51fa22a4755"
                + "699b2a714fcdc6f83766e5f97b6c7423"
                + "73806900e49f24b22b097544d4896b42"
                + "4989b5e1ebac0f07c23f4598";

        String t_hex = "3612d2e79e3b0785561be14aaca2fccb";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase5");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase5");
        return errNo;
    }

    public static int nist_gcm_case6() {
        String k_hex = "feffe9928665731c6d6a8f9467308308";

        String p_hex = "d9313225f88406e5a55909c5aff5269a"
                + "86a7a9531534f7da2e4c303d8a318a72"
                + "1c3c0c95956809532fcf0e2449a6b525"
                + "b16aedf5aa0de657ba637b39";

        String iv_hex = "9313225df88406e555909c5aff5269aa"
                + "6a7a9538534f7da1e4c303d2a318a728"
                + "c3c0c95156809539fcf0e2429a6b5254"
                + "16aedbf5a0de6a57a637b39b"; // > 96bits

        String aad_hex = "feedfacedeadbeeffeedfacedeadbeef"
                + "abaddad2";
        //expected result
        String c_hex = "8ce24998625615b603a033aca13fb894"
                + "be9112a5c3a211a8ba262a3cca7e2ca7"
                + "01e4a9a4fba43c90ccdcb281d48c7c6f"
                + "d62875d2aca417034c34aee5";

        String t_hex = "619cc5aefffe0bfa462af43c1699d050";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase6");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase6");
        return errNo;
    }

    public static int nist_gcm_case7() {
        String k_hex = "00000000000000000000000000000000"
                + "0000000000000000";
        String p_hex = "";
        String iv_hex = "000000000000000000000000";
        String aad_hex = "";

        //expected result
        String c_hex = "";
        String t_hex = "cd33b28ac773f74ba00ed1f312572435";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase7");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase7");
        return errNo;
    }

    public static int nist_gcm_case8() {
        String k_hex = "00000000000000000000000000000000"
                + "0000000000000000";
        String p_hex = "00000000000000000000000000000000";
        String iv_hex = "000000000000000000000000";
        String aad_hex = "";

        //expected result
        String c_hex = "98e7247c07f0fe411c267e4384b0f600";
        String t_hex = "2ff58d80033927ab8ef4d4587514f0fb";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase8");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase8");
        return errNo;
    }

    public static int nist_gcm_case9() {
        String k_hex = "feffe9928665731c6d6a8f9467308308"
                + "feffe9928665731c";
        String p_hex = "d9313225f88406e5a55909c5aff5269a"
                + "86a7a9531534f7da2e4c303d8a318a72"
                + "1c3c0c95956809532fcf0e2449a6b525"
                + "b16aedf5aa0de657ba637b391aafd255";
        String iv_hex = "cafebabefacedbaddecaf888";
        String aad_hex = "";

        //expected result
        String c_hex = "3980ca0b3c00e841eb06fac4872a2757"
                + "859e1ceaa6efd984628593b40ca1e19c"
                + "7d773d00c144c525ac619d18c84a3f47"
                + "18e2448b2fe324d9ccda2710acade256";
        String t_hex = "9924a7c8587336bfb118024db8674a14";

        int errNo = 0;
        errNo += runEncryptCase(k_hex, p_hex, iv_hex, aad_hex, c_hex, t_hex, "TestCase9");
        errNo += runDecryptCase(k_hex, c_hex + t_hex, iv_hex, aad_hex, p_hex, t_hex, "TestCase9");
        return errNo;
    }

    static private int runEncryptCase(String k_hex, String p_hex, String iv_hex, String aad_hex, String c_hex, String t_hex, String hint) {
        byte[] K = hexStrToBytes(k_hex);
        byte[] P = hexStrToBytes(p_hex);
        byte[] IV = hexStrToBytes(iv_hex);
        byte[] AAD = hexStrToBytes(aad_hex);

        byte[] C = hexStrToBytes(c_hex);
        byte[] T = hexStrToBytes(t_hex);

        byte[] output = gcmEncrypt(K, P, AAD, IV);

        byte[] cipher = new byte[output.length - BLOCK_SIZE];
        byte[] tag = new byte[BLOCK_SIZE];
        System.arraycopy(output, 0, cipher, 0, output.length - BLOCK_SIZE);
        System.arraycopy(output, output.length - BLOCK_SIZE, tag, 0, BLOCK_SIZE);

        System.out.println("-------------------------------------");
        int errNo = 0;
        errNo = compareArrays(cipher, C);
        if (errNo > 0) {
            System.out.println(hint + " data encryption failed!");
        } else {
            System.out.println(hint + " data encryption passed!");
        }

        if (compareArrays(tag, T) > 0) {
            errNo++;
            System.out.println(hint + " tag verification failed!");
        } else {
            System.out.println(hint + " tag verification passed!");
        }

        return errNo;
    }

    static private int runDecryptCase(String k_hex, String c_hex, String iv_hex, String aad_hex, String p_hex, String t_hex, String hint) {
        //return runCryptCase(false, k_hex, c_hex, iv_hex, aad_hex, p_hex, t_hex, hint);

        byte[] K = hexStrToBytes(k_hex);
        byte[] P = hexStrToBytes(p_hex);
        byte[] IV = hexStrToBytes(iv_hex);
        byte[] AAD = hexStrToBytes(aad_hex);

        byte[] C = hexStrToBytes(c_hex);
        byte[] T = hexStrToBytes(t_hex);

        byte[] output;
        try {
            output = gcmDecrypt(K, C, AAD, IV);
        } catch (AEADBadTagException e) {
            System.out.println("data is corrupted!");
            return 1;
        }

        System.out.println("-------------------------------------");
        int errNo = 0;
        errNo = compareArrays(output, P);
        if (errNo > 0) {
            System.out.println(hint + " data decryption failed!");
        } else {
            System.out.println(hint + " data decryption passed!");
        }

        return 0;
    }

    private static byte[] gcmEncrypt(byte[] key, byte[] plainText, byte[] aad, byte[] iv) {
        byte[] cipherText = new byte[plainText.length + 16];
        try {

            SecretKeySpec keySpec = new SecretKeySpec(key, ALGORITHM);
            GCMParameterSpec gcmSpec = new GCMParameterSpec(GCM_TAG_LENGTH * 8, iv);
            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");
            cipher.init(Cipher.ENCRYPT_MODE, keySpec, gcmSpec);

            if (aad != null && aad.length != 0) {
                cipher.updateAAD(aad);
            }

            //if has more than one blocks, test cipher.update
            if (plainText.length > 16) {
                cipher.update(plainText, 0, 16, cipherText, 0);
                cipher.doFinal(plainText, 16, plainText.length - 16, cipherText, 16);
            } else {
                //doFinal(byte[] input, int inputOffset, int inputLen, byte[] output, int outputOffset)
                cipher.doFinal(plainText, 0, plainText.length, cipherText, 0);
            }

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return cipherText;
    }

    private static byte[] gcmDecrypt(byte[] key, byte[] cipherText, byte[] aad, byte[] iv) throws AEADBadTagException {
        byte[] plainText = new byte[cipherText.length - BLOCK_SIZE];
        try {

            SecretKeySpec keySpec = new SecretKeySpec(key, ALGORITHM);
            GCMParameterSpec gcmSpec = new GCMParameterSpec(GCM_TAG_LENGTH * 8, iv);
            Cipher cipher = Cipher.getInstance(ALGORITHM + "/" + MODE + "/NoPadding");
            cipher.init(Cipher.DECRYPT_MODE, keySpec, gcmSpec);

            if (aad != null && aad.length != 0) {
                cipher.updateAAD(aad);
            }

            //store internally until decryptFinal is called            
            cipher.doFinal(cipherText, 0, cipherText.length, plainText, 0);

        } catch (AEADBadTagException e) {
            System.out.println(e.getMessage());
            throw e;
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return plainText;
    }

    // "cafebabe" -> {0xca, 0xfe, 0xba, 0xbe}
    static private byte[] hexStrToBytes(String hexStr) {
        if (hexStr == null || hexStr.length() == 0) {
            return new byte[0];
        }
        byte[] out = new byte[hexStr.length() / 2];
        String str = hexStr.toLowerCase();

        byte high;
        byte low;
        for (int i = 0; i < out.length; i++) {
            if (str.charAt(i * 2) >= 'a' && str.charAt(i * 2) <= 'z') {
                high = (byte) (str.charAt(i * 2) - 'a' + 10);
            } else {
                high = (byte) (str.charAt(i * 2) - '0');
            }

            if (str.charAt(i * 2 + 1) >= 'a' && str.charAt(i * 2 + 1) <= 'z') {
                low = (byte) (str.charAt(i * 2 + 1) - 'a' + 10);
            } else {
                low = (byte) (str.charAt(i * 2 + 1) - '0');
            }
            out[i] = (byte) (high * 16 + low);
        }
        return out;
    }

    static private byte[] convertToBytes(char[] in) {
        byte[] out = new byte[in.length];
        for (int i = 0; i < in.length; i++) {
            out[i] = (byte) in[i];
        }
        return out;
    }

    static private void showArray(byte b[], String name) {
        System.out.format("%s [%d]: ", name, b.length);
        for (int i = 0; i < b.length; i++) {
            System.out.format("0x%02x, ", b[i] & 0xff);
        }
        System.out.println();
    }

    static private int compareArrays(byte b[], byte exp[]) {
        if (b.length != exp.length) {
            System.out.format("different lengths for actual and expected output arrays\n");
            showArray(b, "test: ");
            showArray(exp, "exp : ");
            return 1;
        }
        for (int i = 0; i < exp.length; i++) {
            if (b[i] != exp[i]) {
                System.out.format("output error at index %d: got %02x, expected %02x\n", i, b[i] & 0xff, exp[i] & 0xff);
                showArray(b, "test: ");
                showArray(exp, "exp : ");
                return 1;
            }
        }
        return 0; // the same
    }
}
