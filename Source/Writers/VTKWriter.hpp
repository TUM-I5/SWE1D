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

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include "Tools/RealType.hpp"

namespace Writers {

  /**
   * A writer class that generates VTK files
   */
  class VTKWriter {
  private:
    // Base name of the VTP collections and VTK files
    std::string basename_;

    RealType cellSize_;

    // Current time step
    unsigned int timeStep_;

    // VTP stream
    std::ofstream* vtpFile_;

    /**
     * @return The generated filename containing the time step and the real name
     */
    std::string generateFileName();

  public:
    VTKWriter(const std::string& basename = "SWE1D", const RealType cellSize = 1);
    ~VTKWriter();

    /**
     * Writes all values to VTK file
     *
     * @param size Number of cells (without boundary values)
     */
    void write(const RealType time, const RealType* h, const RealType* hu, unsigned int size);
  };

} // namespace Writers
