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
		byte col = 27;
		byte lin = 15;
		char c = 'X';
		
		MSX.setCursor(col, lin);
		MSX.putChar(c);
		
		col = 1; lin = 1;
		MSX.setCursor(col, lin);
		MSX.putChar(c);
		
		while(true);
	}
}
