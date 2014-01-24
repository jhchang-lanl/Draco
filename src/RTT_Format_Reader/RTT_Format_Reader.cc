//----------------------------------*-C++-*--------------------------------//
/*! 
 * \file   RTT_Format_Reader/RTT_Format_Reader.cc
 * \author B.T. Adams
 * \date   Wed Jun 7 10:33:26 2000
 * \brief  Implementation file for RTT_Format_Reader library.
 * \note   Copyright (C) 2000-2014 Los Alamos National Security, LLC. 
 *         All rights reserved. 
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#include <sstream>
#include "RTT_Format_Reader.hh"
#include "ds++/Assert.hh"

namespace rtt_RTT_Format_Reader
{
/*!
 * \brief Constructs an RTT_Format_Reader object and parses the mesh data.
 * \param RTT_File Mesh file name.
 */
RTT_Format_Reader::RTT_Format_Reader(string const & RTT_File)
    : header(),
      dims(),
      spNodeFlags(NULL),
      spSideFlags(NULL),
      spCellFlags(NULL),
      spNodeDataIds(NULL),
      spSideDataIds(NULL),
      spCellDataIds(NULL),
      spCellDefs(NULL),
      spNodes(NULL),
      spSides(NULL),
      spCells(NULL),
      spNodeData(NULL),
      spSideData(NULL),
      spCellData(NULL)
{
    readMesh(RTT_File);
}
/*!
 * \brief Parses the mesh file data via calls to the member data class objects
 *        public "read" member functions.
 * \param RTT_File Mesh file name.
 * \throw invalid_argument If the file cannot be opened
 */
void RTT_Format_Reader::readMesh(const string & RTT_File)
{
    const char * file = RTT_File.c_str();
    ifstream meshfile(file, std::ios::in);
    if (!meshfile)
    {
        std::ostringstream buffer;
        buffer << "File " << RTT_File << " could not be opened\n";
        throw std::invalid_argument(buffer.str());
    }

    try
    {
        readKeyword(meshfile);
        header.readHeader(meshfile);
        dims.readDims(meshfile);
        createMembers();
        readFlagBlocks(meshfile);
        readDataIDs(meshfile);
        spCellDefs->readCellDefs(meshfile);
        spNodes->readNodes(meshfile);
        spSides->readSides(meshfile);
        spCells->readCells(meshfile);
        spNodeData->readNodeData(meshfile);
        spSideData->readSideData(meshfile);
        spCellData->readCellData(meshfile);
        readEndKeyword(meshfile);
    }
    catch (rtt_dsxx::assertion as)
    {
        std::cout << "Assertion thrown: " << as.what() << std::endl;
        Insist(false, as.what());
    }

}
/*!
 * \brief Reads and validates the magic cookie at the beginning of the mesh 
 *        file.
 * \param meshfile Mesh file name.
 */
void RTT_Format_Reader::readKeyword(ifstream & meshfile)
{
    string dummyString;

    meshfile >> dummyString;
    Insist(dummyString == "rtt_ascii", "Invalid mesh file: Not an RTT file");
    std::getline(meshfile, dummyString);       // read and discard blank line.
}

/*!
 * \brief Instantiates the majority of the RTT_Format_Reader member data class
 *        objects.
 */
void RTT_Format_Reader::createMembers()
{
    spNodeFlags = new NodeFlags(dims);
    spSideFlags = new SideFlags(dims);
    spCellFlags = new CellFlags(dims);
    spNodeDataIds = new NodeDataIDs(dims);
    spSideDataIds = new SideDataIDs(dims);
    spCellDataIds = new CellDataIDs(dims);
    spCellDefs = new CellDefs(dims);
    spNodes = new Nodes(* spNodeFlags, dims);
    spSides = new Sides(* spSideFlags, dims, * spCellDefs);
    spCells = new Cells(* spCellFlags, dims, * spCellDefs);
    spNodeData = new NodeData(dims);
    spSideData = new SideData(dims);
    spCellData = new CellData(dims);
}

/*!
 * \brief Reads the node, side, and cell flag blocks from the mesh file.
 * \param meshfile Mesh file name.
 */
void RTT_Format_Reader::readFlagBlocks(ifstream & meshfile)
{
    spNodeFlags->readNodeFlags(meshfile);
    spSideFlags->readSideFlags(meshfile);
    spCellFlags->readCellFlags(meshfile);
}
/*!
 * \brief Reads the node, side, and cell data_id blocks from the mesh file.
 * \param meshfile Mesh file name.
 */
void RTT_Format_Reader::readDataIDs(ifstream & meshfile)
{
    spNodeDataIds->readDataIDs(meshfile);
    spSideDataIds->readDataIDs(meshfile);
    spCellDataIds->readDataIDs(meshfile);
}
/*!
 * \brief Reads and validates the end_rtt_mesh keyword at the end of the mesh 
 *        file.
 * \param meshfile Mesh file name.
 */
void RTT_Format_Reader::readEndKeyword(ifstream & meshfile)
{
    string dummyString;

    meshfile >> dummyString;
    Insist(dummyString == "end_rtt_mesh",
	   "Invalid mesh file: RTT file missing end");
    std::getline(meshfile, dummyString);
}
/*!
 * \brief Transforms the RTT_Format data to an alternative coordinate-system 
 *        independent format.
 * \param cell_side_types New side types for each of the existing cell 
 *        definitions.
 * \param cell_ordered_sides New ordered sides for each of the existing cell 
 *        definitions.
 */
void RTT_Format_Reader::reformatData(
    vector_vector_int const & cell_side_types,
    std::vector< std::vector< std::vector< size_t > > >
    const & cell_ordered_sides)
{
    spCellDefs->redefineCellDefs(cell_side_types, cell_ordered_sides);
    spSides->redefineSides();
    spCells->redefineCells();
}

} // end namespace rtt_RTT_Format_Reader

//---------------------------------------------------------------------------//
// end of RTT_Format_Reader/RTT_Format_Reader.cc
//---------------------------------------------------------------------------//
