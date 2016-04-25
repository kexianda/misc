
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author xiandake
 */
public class TestGCMCipherInputStream {
    public static void main(String[] args) throws Exception {
        TestCipherInputStream();
    }
    
    private static void TestCipherInputStream() {
        try {
            Cipher c = Cipher.getInstance("AES/GCM/NoPadding", "SunJCE");
            Key key = new SecretKeySpec(new byte[16], "AES");
            byte[] iv = new byte[16];
            c.init(Cipher.ENCRYPT_MODE, key, new GCMParameterSpec(128, iv));
            
            byte[] pt = new byte[16*8];
            for(int i=0; i< 16*8; i++) {
                pt[i] = (byte) (i & 0xff);
            }
            byte[] ct = c.doFinal(pt);

            // Tamper with ciphertext - should fail tag check on decrypt
            ct[0] = (byte) (ct[0] + 1);

            c.init(Cipher.DECRYPT_MODE, key, new GCMParameterSpec(128, iv));

            CipherInputStream cin = new CipherInputStream(new ByteArrayInputStream(ct), c);

            // Read until no more data, forces Cipher.doFinal() in getMoreData()
            int count = 0;
            while (cin.read() != -1) {
                count++;
            }
            // Don't close due to other bug in CipherInputStream with AEAD ciphers
            // cin.close();
            System.err.println(count);
        } catch(IOException ex){
            System.err.println(ex.getMessage());
        } catch(NoSuchAlgorithmException | NoSuchProviderException | NoSuchPaddingException | InvalidKeyException | InvalidAlgorithmParameterException | IllegalBlockSizeException | BadPaddingException e){
            System.err.println(e.getMessage());
        }
    }
}
