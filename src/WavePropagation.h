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

#ifndef WAVEPROPAGATION_H_
#define WAVEPROPAGATION_H_

// Include the solver that is currently used
#define SUPPRESS_SOLVER_DEBUG_OUTPUT
#include "solvers/FWave.hpp"

/**
 * Allocated variables:
 *   unknowns h,hu are defined on grid indices [0,..,n+1] (done by the caller)
 *     -> computational domain is [1,..,nx]
 *     -> plus ghost cell layer
 *
 *   net-updates are defined for edges with indices [0,..,n]
 *
 * A left/right net update with index (i-1) is located on the edge between
 *   cells with index (i-1) and (i):
 * <pre>
 *   *********************
 *   *         *         *
 *   *  (i-1)  *   (i)   *
 *   *         *         *
 *   *********************
 *
 *             *
 *            ***
 *           *****
 *             *
 *             *
 *    NetUpdatesLeft(i-1)
 *             or
 *    NetUpdatesRight(i-1)
 * </pre>
 */
class WavePropagation
{
private:
	float *m_h;
	float *m_hu;

	float *m_hNetUpdatesLeft;
	float *m_hNetUpdatesRight;

	float *m_huNetUpdatesLeft;
	float *m_huNetUpdatesRight;

	unsigned int m_size;

	float m_cellSize;

	/** The solver used in computeNumericalFluxes */
	solver::FWave<float> m_solver;

public:
	/**
	 * @param size Domain size (= number of cells) without ghost cells
	 * @param cellSize Size of one cell
	 */
	WavePropagation(float *h, float *hu, unsigned int size, float cellSize)
		: m_h(h),
		  m_hu(hu),
		  m_size(size),
		  m_cellSize(cellSize)
	{
		// Allocate net updates
		m_hNetUpdatesLeft = new float[size+1];
		m_hNetUpdatesRight = new float[size+1];
		m_huNetUpdatesLeft = new float[size+1];
		m_huNetUpdatesRight = new float[size+1];
	}

	~WavePropagation()
	{
		// Free allocated memory
		delete [] m_hNetUpdatesLeft;
		delete [] m_hNetUpdatesRight;
		delete [] m_huNetUpdatesLeft;
		delete [] m_huNetUpdatesRight;
	}

	/**
	 * Computes the net-updates from the unknowns
	 */
	float computeNumericalFluxes();

	/**
	 * Update the unknowns with the already computed net-updates
	 *
	 * @param dt Time step size
	 */
	void updateUnknowns(float dt);

	/**
	 * Updates h and hu according to the outflow condition to both
	 * boundaries
	 */
	void setOutflowBoundaryConditions();
};


#endif /* WAVEPROPAGATION_H_ */
