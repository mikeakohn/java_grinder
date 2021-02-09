/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2014-2021 by Michael Kohn
 *
 */

#ifndef JAVA_GRINDER_GENERATOR_API_INTELLIVISION_H
#define JAVA_GRINDER_GENERATOR_API_INTELLIVISION_H

class API_Intellivision
{
public:
  virtual int intellivision_plot_II() { return -1; }
  virtual int intellivision_plot_III() { return -1; }
};

#endif

