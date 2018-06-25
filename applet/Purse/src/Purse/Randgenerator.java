package Purse;

import javacard.framework.JCSystem;
import javacard.framework.Util;
import javacard.security.RandomData;

public class Randgenerator {
	private byte size;
	private byte[] v; //随机数的值
	private RandomData rd;
	
	public Randgenerator() {
		size = (byte)4;
		v = JCSystem.makeTransientByteArray((short)4, JCSystem.CLEAR_ON_DESELECT);
		rd = RandomData.getInstance(RandomData.ALG_SECURE_RANDOM);
	}
	
	public void GenerateSecureRnd() {
		rd.generateData(v, (short)0, (short)size);
	}
	
	public byte getRndValue(byte[] buffer, short bOff) {
		Util.arrayCopyNonAtomic(v, (short)0, buffer, bOff, (short)size);
		return size;
	}
}
