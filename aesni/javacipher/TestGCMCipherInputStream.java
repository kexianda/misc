
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.util.Arrays;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.ShortBufferException;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.IvParameterSpec;
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
        //TestUpdateAADapi();
        //TestGCMapi();
        
        //testByteBuffer();
    }
    
    private static void TestCipherInputStream() throws ShortBufferException {
        
        byte[] pt = new byte[1024];   
        for(int i = 0; i<pt.length;i++) {
            pt[i] = (byte) ((i + (int)'a') % 26);
        }
        byte[] k = new byte[16];        
        byte[] iv = new byte[16];    //12, 16, ... ?
        byte[] aad = new byte[12];  
        
        byte[] ct = new byte[pt.length + 16];
        
        try {
            
            // ---------------------------------------------------------------------
            Cipher c = Cipher.getInstance("AES/GCM/NoPadding", "SunJCE");
            Key key = new SecretKeySpec(k, "AES");           
            c.init(Cipher.ENCRYPT_MODE, key, new GCMParameterSpec(128, iv));          
            c.updateAAD(aad);
            //c.update(pt, 0, 16, ct, 0);  // encrypt first block,
            //c.doFinal(pt, 0, pt.length, ct, 0); 
            ct = c.doFinal(pt);
            
            //----------------------------
            c.init(Cipher.DECRYPT_MODE, new SecretKeySpec(k, "AES"), new GCMParameterSpec(128, iv));
            c.updateAAD(aad);
            byte[] decOut = new byte[1024];
            //c.update(ct, 0,         256, decOut, 0);
            //c.update(ct, 256,       256, decOut, 256);
            //c.update(ct, 256 * 2,   256, decOut, 256 * 2);
            //c.update(ct, 256 * 3,   256, decOut, 256 * 3);
            //c.doFinal(ct, 256 * 4,  16, decOut, 0);  //verify tag
            
            //c.update(ct, 0, ct.length);  //c.doFinal()  or c.doFinal(decOut);
            //c.doFinal(ct, ct.length, 0, decOut);
            
            ByteBuffer in = ByteBuffer.allocateDirect(ct.length);
            in.put(ct);
            in.flip();
            ByteBuffer out = ByteBuffer.allocateDirect(pt.length);
            c.update(in, out);
            c.doFinal(in, out);
            
            // ---------------------------------------------------------------------
            
            // Tamper with ciphertext - should fail tag check on decrypt
            //ct[0] = (byte) (ct[0] + 1);
            //key.getEncoded();

            Cipher d = Cipher.getInstance("AES/GCM/NoPadding", "SunJCE");
            d.init(Cipher.DECRYPT_MODE, new SecretKeySpec(k, "AES"), new GCMParameterSpec(128, iv));
            d.updateAAD(aad);
            CipherInputStream cin = new CipherInputStream(new ByteArrayInputStream(ct), d);
            
            byte[] tmp = new byte[256];
            while(cin.read(tmp)>0) {
                
            }
            //cin.readAllBytes();

            // Read until no more data, forces Cipher.doFinal() in getMoreData()
            //int count = 0;
            // while (cin.read() != -1) {
            //    count++;
            //}
            // Don't close due to other bug in CipherInputStream with AEAD ciphers
            // cin.close();
            //System.err.println(count);
        } catch(IOException ex){
            System.err.println(ex.getMessage());
        } catch(NoSuchAlgorithmException | NoSuchProviderException | NoSuchPaddingException | InvalidKeyException | InvalidAlgorithmParameterException | IllegalBlockSizeException | BadPaddingException e){
            System.err.println(e.getMessage());
        }
    }
    
    private static void TestUpdateAADapi() {
        byte[] pt = new byte[16 * 8];
        for (int i = 0; i < 16 * 8; i++) {
            pt[i] = (byte) (i & 0xff);
        }

        byte[] k = new byte[16];
        byte[] iv = new byte[16];    //12, 16, ... ?
        byte[] aad = new byte[12];

        byte[] ct = new byte[pt.length + 16];

        try {
            Cipher c = Cipher.getInstance("AES/CTR/NoPadding", "SunJCE");
            Key key = new SecretKeySpec(k, "AES");

            c.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(iv));

            //c.updateAAD(aad);
            
            c.update(pt, 0, 16, pt, 0);  // encrypt first block,
            c.doFinal(pt, 0, pt.length-16, ct, 16);

        } catch (NoSuchAlgorithmException 
                | NoSuchProviderException 
                | NoSuchPaddingException 
                | InvalidKeyException 
                | InvalidAlgorithmParameterException 
                | ShortBufferException 
                | IllegalBlockSizeException 
                | BadPaddingException ex) {
            System.err.println(ex.getMessage());
        } 
    }
    
    private static void TestGCMapi() {
        byte[] pt = new byte[16 * 8];
        for (int i = 0; i < 16 * 8; i++) {
            pt[i] = (byte) (i & 0xff);
        }

        byte[] k = new byte[16];
        byte[] iv = new byte[16];    //12, 16, ... ?
        byte[] aad = new byte[12];

        //byte[] ct = new byte[pt.length + 16];

        try {
            //encryption
            Cipher c = Cipher.getInstance("AES/GCM/NoPadding", "SunJCE");
            Key key = new SecretKeySpec(k, "AES");
            c.init(Cipher.ENCRYPT_MODE, key, new GCMParameterSpec(128,iv));
            c.updateAAD(aad);
            byte[] encOutput = c.doFinal(pt);
            
            encOutput[0] = (byte)(encOutput[0] + 1);
            
            // decryption
            byte[] ct = encOutput;
            byte[] decOutput = new byte[pt.length];
            Cipher d = Cipher.getInstance("AES/GCM/NoPadding", "SunJCE");
            d.init(Cipher.DECRYPT_MODE, new SecretKeySpec(k, "AES"), new GCMParameterSpec(128, iv));
            ByteBuffer bfAAD = ByteBuffer.allocateDirect(aad.length);
            bfAAD.put(aad);
            bfAAD.flip();
            d.updateAAD(bfAAD);
            d.doFinal(ct, 0, ct.length, decOutput, 0);
            
        } catch (Exception ex) {
            System.err.println(ex.getMessage());
        }
    }
    
    private static void testByteBuffer() {
    
        ByteBuffer directBuf = ByteBuffer.allocateDirect(128);
        directBuf.isDirect();
        //byte[] arr = directBuf.array();
                
        {
            ByteBuffer bfBigEndian = ByteBuffer.allocate(4);
            bfBigEndian.order(ByteOrder.BIG_ENDIAN);
            bfBigEndian.putInt(88);
            byte[] resBigEndian = bfBigEndian.array();
            System.out.println(Arrays.toString(resBigEndian));
            
            ByteBuffer bfLitteEndian = ByteBuffer.allocate(12);
            bfLitteEndian.order(ByteOrder.nativeOrder());//ByteOrder.LITTLE_ENDIAN
            bfLitteEndian.putInt(88);
            byte[] resLitteEndian = bfLitteEndian.array();
            resLitteEndian[2] = (byte) 1;
            bfLitteEndian.putInt(99);

            System.out.println(Arrays.toString(resLitteEndian));
        
        }
    }
}
