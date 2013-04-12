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

#ifndef VTKWRITER_H_
#define VTKWRITER_H_

#include "types.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

namespace writer
{

/**
 * A writer class that generates vtk files
 */
class VtkWriter
{
private:
	// base name of the vtp collectiond and vtk files
	std::string m_basename;

	// cell size
	T m_cellSize;

	// current time step
	unsigned int m_timeStep;

	// vtp stream
	std::ofstream *m_vtpFile;


public:
	// constructor
	VtkWriter( const std::string& basename = "swe1d", const T cellSize = 1)
		: m_basename(basename),
                  m_cellSize(cellSize),
		  m_timeStep(0)
	{
		// initialize vtp stream
		std::ostringstream l_vtpFileName;
		l_vtpFileName << m_basename << ".vtp";

		m_vtpFile = new std::ofstream( l_vtpFileName.str().c_str() );

		// write vtp header
                *m_vtpFile
			<< "<?xml version=\"1.0\"?>" << std::endl
			<< "<VTKFile type=\"Collection\" version=\"0.1\">" << std::endl
  			<< "<Collection>" << std::endl;

	}

	// destructor (free memory)
	~VtkWriter() {
		// close vtp file
                *m_vtpFile
			<< "</Collection>" << std::endl
			<< "</VTKFile>" << std::endl;
		delete m_vtpFile;	
	}

	/**
	 * Writes all values to vtk file
	 *
	 * @param size Number of cells (without boundary values)
	 */
	void write(const T time, const T *h, const T *hu, unsigned int size)
	{
		// generate vtk file name
		std::string l_fileName = generateFileName();

		// add current time to vtp collection
		*m_vtpFile << "<DataSet timestep=\""
                           << time
                           << "0\" group=\"\" part=\"0\" file=\""
                           << l_fileName
                           << "\"/> " << std::endl;

		// write vtk file
		std::ofstream vtkFile(l_fileName.c_str());
		assert(vtkFile.good());

		// vtk xml header
		vtkFile << "<?xml version=\"1.0\"?>" << std::endl
				<< "<VTKFile type=\"RectilinearGrid\">" << std::endl
				<< "<RectilinearGrid WholeExtent=\"0 " << size
					<< " 0 0 0 0\">" << std::endl
		        << "<Piece Extent=\"0 " << size
		        	<< " 0 0 0 0\">" << std::endl;

		vtkFile << "<Coordinates>" << std::endl
			<< "<DataArray type=\"Float32\" format=\"ascii\">" << std::endl;

		// grid points
		for (int i=0; i < size+1; i++)
			vtkFile << m_cellSize * i << "" << std::endl;

		vtkFile << "</DataArray>" << std::endl;

		vtkFile	<< "<DataArray type=\"Float32\" format=\"ascii\">" << std::endl
				<< "0" << std::endl
				<< "</DataArray>" << std::endl;

		vtkFile	<< "<DataArray type=\"Float32\" format=\"ascii\">" << std::endl
				<< "0" << std::endl
				<< "</DataArray>" << std::endl;

		vtkFile << "</Coordinates>" << std::endl;

		vtkFile << "<CellData>" << std::endl;

		// water surface height
		vtkFile << "<DataArray Name=\"h\" type=\"Float32\" format=\"ascii\">" << std::endl;
		for (int i=1; i < size+1; i++)
				vtkFile << h[i] << std::endl;
		vtkFile << "</DataArray>" << std::endl;

		// momentum
		vtkFile << "<DataArray Name=\"hu\" type=\"Float32\" format=\"ascii\">" << std::endl;
		for (int i=1; i < size+1; i++)
			vtkFile << hu[i] << std::endl;
		vtkFile << "</DataArray>" << std::endl;

		// bathymetry
		//vtkFile << "<DataArray Name=\"B\" type=\"Float32\" format=\"ascii\">" << std::endl;
		//for (int i=1; i<size+1; i++)
		//		vtkFile << b[i] << std::endl;
		//vtkFile << "</DataArray>" << std::endl;

		vtkFile << "</CellData>" << std::endl
				<< "</Piece>" << std::endl;

		vtkFile << "</RectilinearGrid>" << std::endl
				<< "</VTKFile>" << std::endl;

		// increment time step
		m_timeStep++;
	}

private:
	/**
	 * @return The generated filename containing the time step and the real name
	 */
	std::string generateFileName()
	{
		std::ostringstream name;
		name << m_basename << '_' << m_timeStep << ".vtr";

		return name.str();
	}
};

}

#endif /* VTKWRITER_H_ */
