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

#include "Logger.hpp"

Tools::Logger Tools::Logger::logger;

Tools::Logger::Logger():
  output_(&std::cout) {}

void Tools::Logger::setOutputStream(std::ostream& output) { output_ = &output; }

void Tools::Logger::log(std::string& message, Level level) { log(message.c_str(), level); }

void Tools::Logger::log(const char* message, Level level) {
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

void Tools::Logger::info(std::string& message) { info(message.c_str()); }

void Tools::Logger::info(const char* message) { *output_ << message << std::endl; }

std::ostream& Tools::Logger::info() { return *output_; }

void Tools::Logger::warning(std::string& message) { warning(message.c_str()); }

void Tools::Logger::warning(const char* message) { *output_ << "Warning: " << message << std::endl; }

std::ostream& Tools::Logger::warning() { return *output_ << "Warning: "; }

void Tools::Logger::error(std::string& message) { error(message.c_str()); }

void Tools::Logger::error(const char* message) {
  // Error messages are always send to std::cerr
  std::cerr << "Error: " << message << std::endl;
  exit(1);
}

Tools::Logger& Tools::Logger::operator<<(std::ostream& (*func)(std::ostream&)) {
  *output_ << func;
  return *this;
}
