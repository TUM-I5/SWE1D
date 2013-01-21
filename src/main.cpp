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

#include "WavePropagation.h"
#include "scenarios/dambreak.h"
#include "writer/ConsoleWriter.h"
#include "tools/args.h"

#include <cstring>

int main(int argc, char** argv)
{
	// Parse command line parameters
	tools::Args args(argc, argv);

	// Scenario
	scenarios::DamBreak scenario(args.size());

	// Allocate memory
	// Water height
	float *h = new float[args.size()+2];
	// Momentum
	float *hu = new float[args.size()+2];

	// Initialize water height and momentum
	for (unsigned int i = 0; i < args.size()+2; i++)
		h[i] = scenario.getHeight(i);
	memset(hu, 0, sizeof(float)*(args.size()+2));

	// Create a writer that is responsible printing out values
	writer::ConsoleWriter writer;

	// Helper class computing the wave propagation
	WavePropagation wavePropagation(h, hu, args.size(), scenario.getCellSize());

	// Write initial data
	tools::Logger::logger.info("Initial data");
	writer.write(h, hu, args.size());

	// Current time of simulation
	float t = 0;

	for (unsigned int i = 0; i < args.timeSteps(); i++) {
		// Do one time step
		tools::Logger::logger << "Computing timestep " << i
				<< " at time " << t << std::endl;

		// Update boundaries
		wavePropagation.setOutflowBoundaryConditions();

		// Compute numerical flux on each edge
		float maxTimeStep = wavePropagation.computeNumericalFluxes();

		// Update unknowns from net updates
		wavePropagation.updateUnknowns(maxTimeStep);

		// Update time
		t += maxTimeStep;

		// Write new values
		writer.write(h, hu, args.size());
	}

	// Free allocated memory
	delete [] h;
	delete [] hu;

	return 0;
}
