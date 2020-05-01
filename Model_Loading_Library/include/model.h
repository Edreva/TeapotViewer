// model.h
#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

/// @file model.h This file contains the declarations for the model class

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vector.h"
#include "cell.h"
#include "material.h"
#include "hexahedron.h"
#include "tetrahedron.h"
#include "pyramid.h"

using namespace std;

/**
 * @brief Class for reading .mod file and providing details regarding its contents
 * @author Ewan Drever-Smith
 * @author Wenxiang LUO
 * @version 1.1
 * @date 05/12/2019
 */
class Model
{
public:
    /// @brief Construct model object from data loaded from a given file
    /// @param filePath Location of .mod file to be loaded
    Model(string filePath);
    /// Default constructor
    Model();
    /// @brief Deallocates memory associated with vector lists
    ~Model();
    /// Copy constructor
    Model(const Model&);
    /// Operator= overload
    const Model& operator=(const Model&);
    /// @brief Outputs the coordinates of each vertice in the model to the console
    /// @return Void
    void displayVertices()const;//outputs to the console each vertice and its coordinate vector (x,y,z)
    /// @brief Outputs the properties of each cell in the model to the console
    /// @return Void
    void displayCells()const;//outputs to the console each cell and its properties (type, volume, weight, density)
    /// @brief Outputs the properties of each material in the model to the console
    /// @return Void
    void displayMaterials()const;//outputs to the console each material in the model and its properties (Density, name colour)
    /// @brief Outputs to the console the overall dimensions of the model
    /// @return void
    void displayDimensions()const;
    /// @brief Gets the total height of the model
    /// @return height of model as double
    double getHeight()const;
    /// @brief Gets the total width of the model
    /// @return width of model as double
    double getWidth()const;
    /// @brief Gets the total depth of the model
    /// @return depth of model as double
    double getDepth()const;
    /// @brief Calculates the centre of the model
    /// @return Calculation result as a Vector point
    Vector getModelCentre()const;
    /// @brief Calculates the total weight of all cells in the model
    /// @return Calculated weight as a double
    double getModelWeight()const;
    /// @brief Gets the number of vertices in the model
    /// @return Number of vertices as a long
    long getNumberOfVertices()const;
    /// @brief Gets the number of cells in the model
    /// @return Number of cells as a long
    long getNumberOfCells()const;
    /// @brief Gets the number of materials in the model
    /// @return Number of materials as a long
    long getNumberOfMaterials()const;
    /// @brief Saves contents of model to a file
    /// @param saveDirectory string containing the path of the .mod file to save to
    void saveModel(string saveDirectory)const;
    /// @brief stream insertion overload
    friend std::ostream& operator<<(std::ostream& out, const Model& m);
    /// @brief gets all the pyramids in a model
    /// @return returns a vector of Pyramid objects
    vector<Pyramid> getPyramid() const;
    /// @brief gets all the hexahedrons in a model
    /// @return returns a vector of hexahedron objects
    vector<Hexahedron> getHex() const;
    /// @brief gets all the tetrahedons in a model
    /// @return returns a vector of tetrahedron objects
    vector<Tetrahedron> getTetra() const;
    /// @brief gets all the vectors in a model
    /// @return a vector of Vector objects
    vector<Vector> getVector() const;

private:
    ///@brief Vector sequence containing all the vector objects in the model
    ///@note Each object in the object list is stored at the index of its ID - This was done for speed in locating the object needed.
    ///However this does have consequences - every ID in the file must be consecutive, or else the object will attempted to be added to a point outside the range of the std::vector.
    ///In the future, if this is no longer acceptable, then store ID as a parameter of the object itself. This would mean that each object could be stored at any point in the list, but would take longer to search for the object with a particular ID.
    vector<Vector> listOfVectors;
    ///@brief Vector sequence containing all the cell objects in the model
    ///@note Each object in the object list is stored at the index of its ID - This was done for speed in locating the object needed.
    ///However this does have consequences - every ID in the file must be consecutive, or else the object will attempted to be added to a point outside the range of the std::vector.
    ///In the future, if this is no longer acceptable, then store ID as a parameter of the object itself. This would mean that each object could be stored at any point in the list, but would take longer to search for the object with a particular ID.
    vector<Cell> listOfCells;
    ///@brief Vector sequence containing all the material objects in the model
    ///@note Each object in the object list is stored at the index of its ID - This was done for speed in locating the object needed.
    ///However this does have consequences - every ID in the file must be consecutive, or else the object will attempted to be added to a point outside the range of the std::vector.
    ///In the future, if this is no longer acceptable, then store ID as a parameter of the object itself. This would mean that each object could be stored at any point in the list, but would take longer to search for the object with a particular ID.
    vector<Material> listOfMaterials;
    ///@brief stores a 'recipe' to generate Cell objects from
    /**
     * @note Table shows how vector and material IDs are stored
     * Cell ID       | [0]:0 | [1]:1 | [n]:n |
     * --------------|-------|-------|-------|
     * [0]: Type     |[0][0] |[0][1] |[0][n] |
     * [1]: Vector 1 |[1][0] |[1][1] |[1][n] |
     * [2]: Vector 2 |[2][0] |[2][1] |[2][n] |
     * [3]: Vector 3 |[3][0] |[3][1] |[3][n] |
     * [4]: Vector 4 |[4][0] |[4][1] |[4][n] |
     * [5]: Vector 5 |[5][0] |[5][1] |[5][n] |
     * [6]: Vector 6 |[6][0] |[6][1] |[6][n] |
     * [7]: Vector 7 |[7][0] |[7][1] |[7][n] |
     * [8]: Vector 8 |[8][0] |[8][1] |[8][n] |
     * [9]: Material |[9][0] |[9][1] |[9][n] |
     */
    vector<vector<int>> uninitCellList;
    ///@brief Populates listOfCells from uninitCellList
    ///@param cellListLength Size of the uninitCellList (how many cells are in the model)
    ///@return void
    ///@note Must be called after the entire file has been read to ensure that each Cell's dependent vectors have been constructed
    void generateCellList(int cellListLength);
    ///@brief Reads vectors from line constructs them and adds them to listOfVectors
    ///@param line line string from file containing the vector parameters
    ///@return void
    void readVector(string line);
    ///@brief Reads cells from line and adds its recipe (IDs of its component Vectors) to uninitCellList
    ///@param line line string from file containing the Cell parameters
    ///@return void
    void readCell(string line);
    ///@brief Reads vectors from line constructs them and adds them to listOfMaterials
    ///@param line line string from file containing the material parameters
    ///@return void
    void readMaterial(string line);
    ///@brief loads the file located at sourceFilePath in model object
    ///@return void
    void loadModel();
    ///@brief Contains the location of the file to be read
    string sourceFilePath;
    ///@brief vector list that contains the all the pyramid cells in the model
    vector<Pyramid> pyramidList;
    ///@brief vector list that contains the all the hexahedron cells in the model
    vector<Hexahedron> hexList;
    ///@brief vector list that contains the all the tetrahedron cells in the model
    vector<Tetrahedron> tetraList;
};
#endif // MODEL_H_INCLUDED
