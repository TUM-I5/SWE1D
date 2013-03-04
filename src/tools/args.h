/**
 * @file
 *  This file is part of SWE1D
 *
 *  SWE1D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SWE1D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SWE1D.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Diese Datei ist Teil von SWE1D.
 *
 *  SWE1D ist Freie Software: Sie koennen es unter den Bedingungen
 *  der GNU General Public License, wie von der Free Software Foundation,
 *  Version 3 der Lizenz oder (nach Ihrer Option) jeder spaeteren
 *  veroeffentlichten Version, weiterverbreiten und/oder modifizieren.
 *
 *  SWE1D wird in der Hoffnung, dass es nuetzlich sein wird, aber
 *  OHNE JEDE GEWAEHELEISTUNG, bereitgestellt; sogar ohne die implizite
 *  Gewaehrleistung der MARKTFAEHIGKEIT oder EIGNUNG FUER EINEN BESTIMMTEN
 *  ZWECK. Siehe die GNU General Public License fuer weitere Details.
 *
 *  Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 *  Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 *
 * @copyright 2013 Technische Universitaet Muenchen
 * @author Sebastian Rettenberger <rettenbs@in.tum.de>
 */

#ifndef TOOLS_ARGS_H_
#define TOOLS_ARGS_H_

#include "tools/logger.h"

#include <getopt.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace tools
{

/**
 * Parse command line arguments
 */
class Args
{
private:
	/** Domain size */
	unsigned int m_size;
	/** Number of time steps we want to simulate */
	unsigned int m_timeSteps;

public:
	Args(int argc, char** argv)
		: m_size(100),
		  m_timeSteps(20.0)
	{
		const struct option longOptions[] = {
			{"size", required_argument, 0, 's'},
			{"time", required_argument, 0, 't'},
			{"help", no_argument, 0, 'h'}
		};

		int c, optionIndex;
		std::stringstream ss;
		while ((c = getopt_long(argc, argv, "s:h",
			longOptions, &optionIndex)) >= 0) {
			switch (c) {
			case 0:
				tools::Logger::logger.error("Could not parse command line arguments");
				break;
			case 's':
				ss.str(optarg);
				ss >> m_size;
				break;
			case 't':
				ss.str(optarg);
				ss >> m_timeSteps;
				break;
			case 'h':
				printHelpMessage();
				exit(0);
				break;
			case '?':
				printHelpMessage(std::cerr);
				abort();
				break;
			default:
				tools::Logger::logger.error("Could not parse command line arguments");
				break;
			}
		}
	}

	unsigned int size()
	{
		return m_size;
	}

	unsigned int timeSteps()
	{
		return m_timeSteps;
	}

private:
	/**
	 * Prints the help message, showing all available options
	 *
	 * @param out The output stream the should be used for
	 *  printing
	 */
	void printHelpMessage(std::ostream &out = std::cout)
	{
		out << "Usage: SWE1D [OPTIONS...]" << std::endl
			<< "  -s, --size=SIZE              domain size" << std::endl
			<< "  -t, --time=TIME              number of simulated time steps" << std::endl
			<< "  -h, --help                   this help message" << std::endl;
	}
};

} /* namespace tools */
#endif /* TOOLS_ARGS_H_ */
