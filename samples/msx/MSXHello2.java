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

public class MSXHello2 {
	public static String strMsg = "Hello MSX from JAVA";

	public static void main(String args[]) {
		MSX.cls();

		for (int i = 0; i < strMsg.length(); i++) {
			MSX.putChar(strMsg.charAt(i));
		}

		while (true);
	}
}
