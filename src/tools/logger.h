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

#ifndef TOOLS_LOGGER_H_
#define TOOLS_LOGGER_H_

#include <cstdlib>
#include <iostream>

namespace tools {

class Logger {
public:
	enum Level { INFO, WARNING, ERROR };

private:
	/** Stream where we print all message */
	std::ostream* m_output;

private:
	Logger()
		: m_output(&std::cout)
	{
	}

public:
	void setOutputStream(std::ostream &output)
	{
		m_output = &output;
	}

	void log(std::string &message, Level level = INFO)
	{
		log(message.c_str(), level);
	}

	void log(const char* message, Level level = INFO)
	{
		switch (level) {
		case INFO:
			info(message);
			break;
		case WARNING:
			warning(message);
			break;
		case ERROR:
			error(message);
			break;
		}
	}

	void info(std::string &message)
	{
		info(message.c_str());
	}

	void info(const char* message)
	{
		*m_output << message << std::endl;
	}

	std::ostream& info()
	{
		return *m_output;
	}

	void warning(std::string &message)
	{
		warning(message.c_str());
	}

	void warning(const char* message)
	{
		*m_output << "Warning: " << message << std::endl;
	}

	std::ostream& warning()
	{
		return *m_output << "Warning: ";
	}

	void error(std::string &message)
	{
		error(message.c_str());
	}

	void error(const char* message)
	{
		// Error messages are always send to std::cerr
		std::cerr << "Error: " << message << std::endl;
		exit(1);
	}

	/**
	 * Can be used to print arbitrary info messages.
	 * Does not append std::endl.
	 */
	template<typename T>
	Logger& operator<<(T value)
	{
		*m_output << value;
		return *this;
	}

	/**
	 * Allow to print std::endl
	 */
	Logger& operator<<(std::ostream& (*func)(std::ostream&))
	{
		*m_output << func;
		return *this;
	}

public:
	static Logger logger;
};

} /* namespace tools */

#endif /* TOOLS_LOGGER_H_ */
