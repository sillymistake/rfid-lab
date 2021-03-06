package Purse;

import javacard.security.DESKey;
import javacard.security.Key;
import javacard.security.KeyBuilder;
import javacardx.crypto.Cipher;

public class PenCipher {
	public Cipher desEngine;
	public Key deskey;
	
	// 构造函数
	public PenCipher() {
		desEngine = Cipher.getInstance(Cipher.ALG_DES_ECB_NOPAD, false);
		deskey = KeyBuilder.buildKey(KeyBuilder.TYPE_DES, KeyBuilder.LENGTH_DES, false);
	}
	
	// des
	public void des(byte[] akey, short kOff, byte mode, byte[] data, short dOff, short dLen, byte[] r, short rOff) {
		((DESKey)deskey).setKey(akey, kOff);
		desEngine.init(deskey, mode);
		desEngine.doFinal(data, dOff, dLen, r, rOff);	
	}
	
	// 生成过程密钥
	public void gen_processkey(byte[] key, byte[] data, short dOff, short dLen, byte[] r, short rOff) {
		// des加密 -> des解密 -> des加密
		des(key, (short)0, Cipher.MODE_ENCRYPT, data, (short)0, dLen, data, (short)0);
		des(key, (short)8, Cipher.MODE_DECRYPT, data, (short)0, dLen, data, (short)0);
		des(key, (short)0, Cipher.MODE_ENCRYPT, data, (short)0, dLen, r, rOff);
	}
	
	public short data_padding(byte[] data, short len) {
		// 末尾补0x80
		data[len] = (byte)0x80;
		len++;
		// 补0x00至为8的倍数
		while(len % 8 != 0) {
			data[len] = (byte)0x00;
			len++;
		}
		return len;
	}
	
	//八位异或 结果放在d1中
	public void xor(byte[] d1, byte[] d2, short d2_off) {
		for(short i = 0; i < 8; i++) {
			d1[i] ^= d2[i+d2_off];
		}
	}
	
	public void gen_mac(byte[] key, byte[] data, short dLen, byte[] mac) {
		byte[] origin_num = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		dLen = data_padding(data, dLen);
		short num = (short)(dLen / 8);
		
		for(short i = 0; i < num; i++) {
			xor(origin_num, data, (short)(i*8));
			des(key, (short)0, Cipher.MODE_ENCRYPT, origin_num, (short)0, (short)8, origin_num, (short)0);
		}
		//取前四个字节
		for(short i = 0; i < 4; i++) {
			mac[i] = origin_num[i];
		}	
	}
	
	
	
	

}
