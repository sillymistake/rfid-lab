package Purse;

import javacard.framework.JCSystem;

public class Papdu {
	public byte pdata[];
	public byte cla, ins, p1, p2, le, lc;
	public byte data[];
	public Papdu() {
		pdata = JCSystem.makeTransientByteArray((short)255, JCSystem.CLEAR_ON_DESELECT);
	}
	public boolean APDUContainData() {
		switch(ins) {
		case condef.INS_CREATE_FILE:
		case condef.INS_LOAD:
		case condef.INS_NIIT_TRANS:
		case condef.INS_WRITE_BIN:
		case condef.INS_WRITE_KEY:
			return true;
		}
		return false;
	}
}
