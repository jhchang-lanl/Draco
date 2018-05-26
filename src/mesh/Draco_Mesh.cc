//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   mesh/Draco_Mesh.cc
 * \date   May 2018
 * \brief  Draco_Mesh class implementation file.
 * \note   Copyright (C) 2018 Los Alamos National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#include "Draco_Mesh.hh"
#include "ds++/Assert.hh"
#include <iostream>

namespace rtt_mesh {

//---------------------------------------------------------------------------//
// CONSTRUCTOR
//---------------------------------------------------------------------------//
/*!
 * \brief Draco_Mesh constructor.
 *
 * \param[in] dimension_ dimension of mesh
 * \param[in] geometry_ enumerator of possible coordinate system geometries
 * \param[in] cell_type_ number of vertices for each cell
 * \param[in] cell_to_node_linkage_ serialized map of cell indices to node
 * indices.
 * \param[in] side_set_flag_ map of side indices (per cell) to side flag (global
 * index for a side).
 * \param[in] side_node_count_ number of nodes per each cell on a side of
 * the mesh.
 * \param[in] side_to_node_linkage_ serialized map of side indices (per side
 * cell) to node indices.
 * \param[in] coordinates_ serialized map of node index to coordinate values.
 * \param[in] global_node_number_ map of local to global node index (vector
 * subscript is local node index and value is global node index; for one
 * process, this is the identity map).
 */
Draco_Mesh::Draco_Mesh(unsigned dimension_, Geometry geometry_,
                       const std::vector<unsigned> &cell_type_,
                       const std::vector<unsigned> &cell_to_node_linkage_,
                       const std::vector<unsigned> &side_set_flag_,
                       const std::vector<unsigned> &side_node_count_,
                       const std::vector<unsigned> &side_to_node_linkage_,
                       const std::vector<double> &coordinates_,
                       const std::vector<unsigned> &global_node_number_)
    : dimension(dimension_), geometry(geometry_), num_cells(cell_type_.size()),
      num_nodes(global_node_number_.size()) {

  // Require(dimension_ <= 3);
  // TODO: generalize mesh generation to 3D (and uncomment requirment above)
  Require(dimension_ <= 2);

  // build the layout (or linkage) of the mesh
  compute_cell_to_cell_linkage(cell_type_, cell_to_node_linkage_);

  // build the cell-face pair to coordinate list map
  compute_cell_face_to_node_coord_map(coordinates_);
}

//---------------------------------------------------------------------------//
// PRIVATE FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Build the cell-face index map to the corresponding coordinates.
 *
 * \param[in] cell_to_node_linkage serialized map of cell indices to node
 * indices (passed from constructor).
 */
void Draco_Mesh::compute_cell_to_cell_linkage(
    const std::vector<unsigned> &cell_type,
    const std::vector<unsigned> &cell_to_node_linkage) {

  // STEP 1: create de-serialized map of cell index to node indices

  std::map<unsigned, std::vector<unsigned>> cell_to_node_map;

  // initialize pointers into cell_to_node_linkage vector
  std::vector<unsigned>::const_iterator cn_first = cell_to_node_linkage.begin();
  for (unsigned cell = 0; cell < num_cells; ++cell) {

    // use the cell_type to create a vector of node indices for this cell
    std::vector<unsigned> node_vec(cn_first, cn_first + cell_type[cell]);

    // increment the map with this cell and vector-of-nodes entry
    cell_to_node_map.insert(std::make_pair(cell, node_vec));

    // update the vector pointer
    cn_first += cell_type[cell];
  }

  Check(cell_to_node_map.size() == num_cells);

  // STEP 2: create the node-to-cell map

  // initialize empty vectors for each node key
  for (unsigned node = 0; node < num_nodes; ++node) {
    std::vector<unsigned> tmp_vec;
    node_to_cell_map.insert(std::make_pair(node, tmp_vec));
  }

  // push cell index to vector for each node
  for (unsigned cell = 0; cell < num_cells; ++cell) {
    for (unsigned node : cell_to_node_map[cell]) {
      node_to_cell_map[node].push_back(cell);
    }
  }

  // STEP 3: identify faces and create cell to face map

  // TODO: global face index?
  // TODO: extend to 3D

  // identify faces per cell
  for (unsigned cell = 0; cell < num_cells; ++cell) {

    // create a vector of all possible node pairs
    unsigned nper_cell = cell_to_node_map[cell].size();
    unsigned num_pairs = nper_cell * (nper_cell - 1) / 2;
    std::vector<std::vector<unsigned>> vec_node_vec(num_pairs,
                                                    std::vector<unsigned>(2));
    // TODO: change type of vec_node_vec?
    unsigned k = 0;
    for (unsigned i = 0; i < nper_cell; ++i) {
      for (unsigned j = i + 1; j < nper_cell; ++j) {

        // set kth pair entry to the size 2 vector of nodes
        vec_node_vec[k] = {cell_to_node_map[cell][i],
                           cell_to_node_map[cell][j]};

        // increment k
        k++;
      }
    }

    Check(k == num_pairs);

    // // check if each pair constitutes a face
    // for (unsigned l = 0; l < num_pairs; ++l) {

    //   // use node-to-cell map to find a common neighbor cell
    // }
  }
}

//---------------------------------------------------------------------------//
/*!
 * \brief Build the cell-face index map to the corresponding coordinates.
 *
 * \param[in] coordinates_ serialized map of node index to coordinate values
 * (passed from constructor).
 */
void Draco_Mesh::compute_cell_face_to_node_coord_map(
    const std::vector<double> &coordinates) {}
} // end namespace rtt_mesh

//---------------------------------------------------------------------------//
// end of mesh/Draco_Mesh.cc
//---------------------------------------------------------------------------//
