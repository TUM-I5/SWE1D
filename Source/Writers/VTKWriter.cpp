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

#include "VTKWriter.hpp"

Writers::VTKWriter::VTKWriter(const std::string& basename, const RealType cellSize):
  basename_(basename),
  cellSize_(cellSize),
  timeStep_(0) {

  // Initialize VTP stream
  std::ostringstream vtpFileName;
  vtpFileName << basename_ << ".vtp";

  vtpFile_ = new std::ofstream(vtpFileName.str().c_str());

  // Write VTP header
  *vtpFile_
    << "<?xml version=\"1.0\"?>" << std::endl
    << "<VTKFile type=\"Collection\" version=\"0.1\">" << std::endl
    << "<Collection>" << std::endl;
}

Writers::VTKWriter::~VTKWriter() {
  // Close VTP file
  *vtpFile_ << "</Collection>" << std::endl << "</VTKFile>" << std::endl;
  delete vtpFile_;
}

void Writers::VTKWriter::write(const RealType time, const RealType* h, const RealType* hu, unsigned int size) {
  // Generate VTK file name
  std::string fileName = generateFileName();

  // Add current time to VTP collection
  *vtpFile_
    << "<DataSet timestep=\"" << time << "0\" group=\"\" part=\"0\" file=\"" << fileName << "\"/> " << std::endl;

  // Write VTK file
  std::ofstream vtkFile(fileName.c_str());
  assert(vtkFile.good());

  // VTL XML header
  vtkFile
    << "<?xml version=\"1.0\"?>" << std::endl
    << "<VTKFile type=\"RectilinearGrid\">" << std::endl
    << "<RectilinearGrid WholeExtent=\"0 " << size << " 0 0 0 0\">" << std::endl
    << "<Piece Extent=\"0 " << size << " 0 0 0 0\">" << std::endl;

  vtkFile << "<Coordinates>" << std::endl << "<DataArray type=\"Float32\" format=\"ascii\">" << std::endl;

  // Grid points
  for (unsigned int i = 0; i < size + 1; i++) {
    vtkFile << cellSize_ * i << "" << std::endl;
  }

  vtkFile << "</DataArray>" << std::endl;

  vtkFile
    << "<DataArray type=\"Float32\" format=\"ascii\">" << std::endl
    << "0" << std::endl
    << "</DataArray>" << std::endl;

  vtkFile
    << "<DataArray type=\"Float32\" format=\"ascii\">" << std::endl
    << "0" << std::endl
    << "</DataArray>" << std::endl;

  vtkFile << "</Coordinates>" << std::endl;

  vtkFile << "<CellData>" << std::endl;

  // Water surface height
  vtkFile << "<DataArray Name=\"h\" type=\"Float32\" format=\"ascii\">" << std::endl;
  for (unsigned int i = 1; i < size + 1; i++) {
    vtkFile << h[i] << std::endl;
  }
  vtkFile << "</DataArray>" << std::endl;

  // Momentum
  vtkFile << "<DataArray Name=\"hu\" type=\"Float32\" format=\"ascii\">" << std::endl;
  for (unsigned int i = 1; i < size + 1; i++) {
    vtkFile << hu[i] << std::endl;
  }
  vtkFile << "</DataArray>" << std::endl;

  vtkFile << "</CellData>" << std::endl << "</Piece>" << std::endl;

  vtkFile << "</RectilinearGrid>" << std::endl << "</VTKFile>" << std::endl;

  // Increment time step
  timeStep_++;
}

std::string Writers::VTKWriter::generateFileName() {
  std::ostringstream name;
  name << basename_ << '_' << timeStep_ << ".vtr";

  return name.str();
}
