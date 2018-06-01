package Purse;

import javacard.framework.APDU;
import javacard.framework.Applet;
import javacard.framework.ISO7816;
import javacard.framework.ISOException;
import javacard.framework.Util;

public class Purse extends Applet {
	
	public Papdu papdu = new Papdu();
	public KeyFile key_file = null;
	public BinaryFile card_file = null;
	public BinaryFile person_file = null;

	public static void install(byte[] bArray, short bOffset, byte bLength) {
		// GP-compliant JavaCard applet registration
		new Purse().register(bArray, (short) (bOffset + 1), bArray[bOffset]);
	}

	public void process(APDU apdu) {
		// Good practice: Return 9000 on SELEfCT
		if (selectingApplet()) {
			return;
		}
		
		short lc = apdu.setIncomingAndReceive();
		byte buffer[];
		buffer = apdu.getBuffer();
		
		papdu.cla = buffer[ISO7816.OFFSET_CLA];
		papdu.ins = buffer[ISO7816.OFFSET_INS];
		papdu.p1 = buffer[ISO7816.OFFSET_P1];
		papdu.p2 = buffer[ISO7816.OFFSET_P2];
		
		if(papdu.APDUContainData()) {
			papdu.lc = buffer[ISO7816.OFFSET_LC];
			papdu.data = new byte[lc];
			Util.arrayCopyNonAtomic(buffer, ISO7816.OFFSET_CDATA, papdu.data, (short)0, lc);
			if(lc == papdu.lc) {
				papdu.le = 0;
			} else {
				papdu.le = buffer[buffer.length-1];
			}
		} else {
			papdu.le = buffer[ISO7816.OFFSET_LC];
			papdu.lc = 0;
		}
		
		boolean rc = handleEvent();
		if(rc && papdu.le != (byte)0) {
			Util.arrayCopyNonAtomic(papdu.data, (short)0, buffer, ISO7816.OFFSET_CDATA, (short)papdu.data.length);
			apdu.setOutgoingAndSend(ISO7816.OFFSET_CDATA, papdu.le);
		}
	}
	
	private boolean handleEvent() {
		switch(papdu.ins) {
		case condef.INS_CREATE_FILE: return create_file();
		case condef.INS_WRITE_BIN: return write_bin();
		case condef.INS_WRITE_KEY: return write_key();
		case condef.INS_READ_BIN: return read_bin();
		}
		ISOException.throwIt(ISO7816.SW_INS_NOT_SUPPORTED);
		return false;
	}
	
	private boolean create_file() {
		switch(papdu.data[0]) {
		case condef.EP_FILE: return EP_file();
		case condef.KEY_FILE: return Key_file();
		case condef.CARD_FILE: return Card_file();
		case condef.PERSON_FILE: return Person_file();
		default:
			ISOException.throwIt(ISO7816.SW_FUNC_NOT_SUPPORTED);
		}
		return true;
	}
	
	private boolean write_key() {
		if(papdu.cla != (byte)0x80) {
			ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
		}
		if(papdu.p1 != (byte)0x00 && papdu.p1 != (byte)0x01) {
			ISOException.throwIt(ISO7816.SW_WRONG_P1P2);
		}
		if(papdu.lc != (byte)0x15) {
			ISOException.throwIt(ISO7816.SW_WRONG_LENGTH);
		}
		if(key_file == null) {
			ISOException.throwIt(ISO7816.SW_CONDITIONS_NOT_SATISFIED);
		}
		key_file.addkey(papdu.p2, papdu.lc, papdu.data);
		return true;
	}
	
	private boolean write_bin() {
		if(papdu.cla != (byte)0x00) {
			ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
		}
		if(papdu.p1 != (byte)0x17 && papdu.p1 != (byte)0x16) {
			ISOException.throwIt(ISO7816.SW_WRONG_P1P2);
		}
		if(papdu.p1 == (byte)0x16) {
			if(card_file != null) {
				card_file.write_bineary(papdu.p2, papdu.lc, papdu.data);
			}
			else {
				ISOException.throwIt(ISO7816.SW_FILE_NOT_FOUND);
			}
		} else if(papdu.p1 == (byte)0x17) {
			if(person_file != null) {
				person_file.write_bineary(papdu.p2, papdu.lc, papdu.data);
			}
			else {
				ISOException.throwIt(ISO7816.SW_FILE_NOT_FOUND);
			}
		}
		return true;
	}
	
	private boolean read_bin() {
		if(papdu.cla != (byte)0x00) {
			ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
		}
		if(papdu.p1 != (byte)0x17 && papdu.p1 != (byte)0x16) {
			ISOException.throwIt(ISO7816.SW_WRONG_P1P2);
		}
		if(papdu.p1 == (byte)0x16) {
			if(card_file != null) {
				card_file.read_binary(papdu.p2, papdu.le, papdu.data);
			}
			else {
				ISOException.throwIt(ISO7816.SW_FILE_NOT_FOUND);
			}
		} else if(papdu.p1 == (byte)0x17) {
			if(person_file != null) {
				person_file.read_binary(papdu.p2, papdu.le, papdu.data);
			}
			else {
				ISOException.throwIt(ISO7816.SW_FILE_NOT_FOUND);
			}
		}
		return true;
	}
	
	private boolean EP_file() {
		return true;
	}
	
	private boolean Key_file() {
		if(papdu.cla != (byte)0x80) {
			ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
		}
		if(papdu.p1 != (byte)0x00) {
			ISOException.throwIt(ISO7816.SW_WRONG_P1P2);
		}
		if(papdu.lc != (byte)0x07) {
			ISOException.throwIt(ISO7816.SW_WRONG_LENGTH);
		}
		key_file = new KeyFile();
		return true;
	}
	private boolean Card_file() {
		if(papdu.cla != (byte)0x80) {
			ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
		}
		if(papdu.p1 != (byte)0x00) {
			ISOException.throwIt(ISO7816.SW_WRONG_P1P2);
		}
		if(papdu.lc != (byte)0x07) {
			ISOException.throwIt(ISO7816.SW_WRONG_LENGTH);
		}
		card_file = new BinaryFile(papdu.data);
		return true;
	}
	private boolean Person_file() {
		if(papdu.cla != (byte)0x80) {
			ISOException.throwIt(ISO7816.SW_CLA_NOT_SUPPORTED);
		}
		if(papdu.p1 != (byte)0x00) {
			ISOException.throwIt(ISO7816.SW_WRONG_P1P2);
		}
		if(papdu.lc != (byte)0x07) {
			ISOException.throwIt(ISO7816.SW_WRONG_LENGTH);
		}
		person_file = new BinaryFile(papdu.data);
		return true;
	}
}
