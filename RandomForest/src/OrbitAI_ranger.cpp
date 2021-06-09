/*-------------------------------------------------------------------------------
 This file is is a copy of the ranger/cpp_version/src/main.cpp source file with
 some csv reading/writing logic added to it for data cleansing prior to training.
 #-------------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <memory>

#include <csv2/reader.hpp>

#include "globals.h"
#include "ArgumentHandler.h"
#include "ForestClassification.h"
#include "ForestRegression.h"
#include "ForestSurvival.h"
#include "ForestProbability.h"
#include "utility.h"

#define INPUT_CSV_FILENAME                                  "ranger_input.csv"

using namespace ranger;
using namespace csv2;

void run_ranger(const ArgumentHandler& arg_handler, std::ostream& verbose_out)
{
  verbose_out << "Starting Ranger." << std::endl;

  /* Create forest object. */
  std::unique_ptr<Forest> forest { };
  switch (arg_handler.treetype) 
  {
    case TREE_CLASSIFICATION:
      if (arg_handler.probability) 
      {
        forest = make_unique<ForestProbability>();
      } 
      else 
      {
        forest = make_unique<ForestClassification>();
      }
      break;
    case TREE_REGRESSION:
      forest = make_unique<ForestRegression>();
      break;
    case TREE_SURVIVAL:
      forest = make_unique<ForestSurvival>();
      break;
    case TREE_PROBABILITY:
      forest = make_unique<ForestProbability>();
      break;
  }

  /* Call Ranger. */
  forest->initCpp(arg_handler.depvarname, arg_handler.memmode, INPUT_CSV_FILENAME, arg_handler.mtry,
      arg_handler.outprefix, arg_handler.ntree, &verbose_out, arg_handler.seed, arg_handler.nthreads,
      arg_handler.predict, arg_handler.impmeasure, arg_handler.targetpartitionsize, arg_handler.splitweights,
      arg_handler.alwayssplitvars, arg_handler.statusvarname, arg_handler.replace, arg_handler.catvars,
      arg_handler.savemem, arg_handler.splitrule, arg_handler.caseweights, arg_handler.predall, arg_handler.fraction,
      arg_handler.alpha, arg_handler.minprop, arg_handler.holdout, arg_handler.predictiontype,
      arg_handler.randomsplits, arg_handler.maxdepth, arg_handler.regcoef, arg_handler.usedepth);

  forest->run(true, !arg_handler.skipoob);
  if (arg_handler.write) 
  {
    forest->saveToFile();
  }

  forest->writeOutput();
  verbose_out << "Finished Ranger." << std::endl;
}

void create_input_csv(std::string inputFile, std::vector<std::string>* pIncludeVars)
{
  /* Create CSV reader. */
  csv2::Reader<delimiter<','>,
      quote_character<'"'>,
      first_row_is_header<true>,
      trim_policy::trim_whitespace> csv;

  if (csv.mmap(inputFile))
  {
    std::ofstream csvInputFile;
    csvInputFile.open(INPUT_CSV_FILENAME);

    /* Read the header row. */
    const auto header = csv.header();

    /* Figure out which columns to process by keeping track of their indices. */
    std::vector<size_t> trainingColumnIndices;
    size_t headerColIndex = 0;
    size_t keptHeaderCounter = 0;
    std::string columnName;

    /* Header row string to write. */
    std::string headerToWrite;

    /* Collect index of columns to process as training or predicting data. */
    for (const auto cell: header)
    {
      /* Get column name. */
      columnName = "";
      cell.read_value(columnName);

      /* Check if that column name is marked to be kept. */
      if (std::find(pIncludeVars->begin(), pIncludeVars->end(), columnName) != pIncludeVars->end())
      {
        /* Increment kept header counter. */
        keptHeaderCounter++;

        /* Keep track of the column index containing data to use as training or predicting data. */
        trainingColumnIndices.push_back(headerColIndex);

        /* Concatenate the string listing the names of columns with training or predicting data. */
        headerToWrite = headerToWrite + columnName;

        /* Include comma separation unless we've reached the last column name to be written. */
        if(keptHeaderCounter != pIncludeVars->size())
        {
          headerToWrite = headerToWrite + ",";
        }
      }

      /* Increment header column index tracker. */
      headerColIndex++;
    }

    /* Write the header row to the new CSV file that will be used as input for ranger. */
    csvInputFile << headerToWrite << std::endl;

    /* Trackers that will be used to skip columns when writing the ranger input csv. */
    /* We skip columns for data that shouldn't be used as training data (e.g. timestamp). */
    size_t rowColIndex;
    std::string rowToWrite;
    std::string cellValue;
    
    /* Traverse each cell of every row. */
    for (const auto row: csv)
    {
      rowColIndex = 0;
      rowToWrite = "";

      for (const auto cell: row)
      {
        /* And only write data from column marked as containing training data. */
        if (std::find(trainingColumnIndices.begin(), trainingColumnIndices.end(), rowColIndex) != trainingColumnIndices.end())
        {
          /* Fetch cell value. */
          cellValue = "";
          cell.read_value(cellValue);

          /* Concatenate the string listing the  training data for the selected columns. */
          rowToWrite = rowToWrite + cellValue;

          /* Include comma separation unless we've reached the last column. */
          if(rowColIndex < trainingColumnIndices.size())
          {
            rowToWrite = rowToWrite + ",";
          }
        }

        /* Increment column index tracker for the current row. */
        rowColIndex++;
      }

      /* Write row to the new CSV file that will be used as input for ranger. */
      csvInputFile << rowToWrite << std::endl;
    }

    /* Done writing to the ranger input CSV file, close it. */
    csvInputFile.close();
  }
}

int main(int argc, char **argv)
{
  try 
  {
    /* Handle command line arguments. */
    ArgumentHandler arg_handler(argc, argv);
    if (arg_handler.processArguments() != 0)
    {
      return 0;
    }
    arg_handler.checkArguments();

    /* The given CSV file can contain columns that should not be used for training or predicting (e.g. timestamp). */
    /* Cleanse the data and create a new CSV file to be used as ranger's input CSV file. */
    create_input_csv(arg_handler.file, &arg_handler.includevars);

    if (arg_handler.verbose)
    {
      run_ranger(arg_handler, std::cout);
    } 
    else 
    {
      std::ofstream logfile { arg_handler.outprefix + ".log" };
      if (!logfile.good()) 
      {
        throw std::runtime_error("Could not write to logfile.");
      }
      run_ranger(arg_handler, logfile);
    }
  } catch (std::exception& e) 
  {
    std::cerr << "Error: " << e.what() << " Ranger will EXIT now." << std::endl;
    return -1;
  }

  return 0;
}
