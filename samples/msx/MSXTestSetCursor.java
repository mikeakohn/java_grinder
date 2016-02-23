/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.naken.cc/
 * License: GPL
 *
 * Copyright 2014-2016 by Michael Kohn
 *
 */

/*
 *   MSX support by Giovanni Nunes - https://github.com/plainspooky
 *                  Emiliano Fraga - https://github.com/efraga-msx
 */

import net.mikekohn.java_grinder.MSX;

/**
 * @author Emiliano
 *
 */
public class MSXTestSetCursor {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		byte mode = 0;
		byte lin = 2;
		byte col = 2;
		char c = '0';
		
		MSX.screen(mode);

		for (int i = 1; i < 41; i++) 
		{
			MSX.setCursor((byte)i, lin);
			MSX.putChar(c++);
			
			if (c > '9') {c = '0';} 
		}

		lin = 1;
		c = '0';
		for (int i = 1; i < 41; i+=10) 
		{
			MSX.setCursor((byte)i, lin);
			MSX.putChar(c++);
		}

		c = '0';
		for (int i = 1; i < 25; i++) 
		{
			MSX.setCursor(col, (byte)i);
			MSX.putChar(c++);
			
			if (c > '9') {c = '0';} 
		}

		col = 1;
		c = '0';
		for (int i = 1; i < 25; i+=10) 
		{
			MSX.setCursor(col, (byte)i);
			MSX.putChar(c++);
		}
		
		c = 'X';
		lin = 3;
		for (int i = 1; i < 21; i++) 
		{
			MSX.setCursor((byte)i, lin++);
			MSX.putChar(c);
		}

		c = 'Y';
		
		for (int i = 21; i < 41; i++) 
		{
			MSX.setCursor((byte)i, lin--);
			MSX.putChar(c);
		}

		while(true);
	}
}
