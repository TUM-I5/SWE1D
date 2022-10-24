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

#pragma once

#include <cstdlib>
#include <iostream>

namespace Tools {

  class Logger {
  public:
    enum Level { INFO, WARNING, ERROR };

  private:
    /** Stream where we print all message */
    std::ostream* output_;

  private:
    Logger();

  public:
    static Logger logger;

    ~Logger() = default;

    void setOutputStream(std::ostream& output);

    void log(std::string& message, Level level = INFO);
    void log(const char* message, Level level = INFO);

    void          info(std::string& message);
    void          info(const char* message);
    std::ostream& info();

    void          warning(std::string& message);
    void          warning(const char* message);
    std::ostream& warning();

    void error(std::string& message);
    void error(const char* message);

    /**
     * Can be used to print arbitrary info messages.
     * Does not append std::endl.
     */
    template <typename T>
    Logger& operator<<(T value) {
      *output_ << value;
      return *this;
    }

    Logger& operator<<(std::ostream& (*func)(std::ostream&));
  };

} // namespace Tools
