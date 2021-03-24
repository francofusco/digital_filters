# Documentation will be optionally built using Doxygen.
find_package(Doxygen QUIET)

# Provide the option to build the documentation.
# Defaults to true if Doxygen is installed.
option(
  BUILD_DOC
  "If enabled, Doxygen will be used to build the documentation."
  ${Doxygen_FOUND}
)

# Provide the option to run Doxygen in quet mode.
set(
  QUIET_DOXYGEN
  YES
  CACHE
  STRING
  "If 'YES', Dxygen will run in quiet mode, thus not producing messages on the standard output (warning and errors are still reported)."
)
set_property(CACHE QUIET_DOXYGEN PROPERTY STRINGS YES NO)

# Provide the option to run Doxygen in quet mode.
set(
  DOXYGEN_MAKE_LATEX
  NO
  CACHE
  STRING
  "If 'YES', Dxygen will create a latex-compilable documentation (in addition to the html output)."
)
set_property(CACHE DOXYGEN_MAKE_LATEX PROPERTY STRINGS YES NO)



# Process the documentation if needed.
if(${BUILD_DOC})
  if(${Doxygen_FOUND})
    # Custom target that will generate figures for documentation purposes
    set(SHOWCASE_DATA_FILE "showcase_data.txt")
    set(SHOWCASE_PNG_FIGURE "showcase.png")
    set(SHOWCASE_PDF_FIGURE "showcase.pdf")
    set(FILTFILT_DATA_FILE "filter2_data.txt")
    set(FILTFILT_PNG_FIGURE "filter2.png")
    set(FILTFILT_PDF_FIGURE "filter2.pdf")
    add_custom_target(
      generate_figures ALL
      COMMAND bash -c "./showcase ${SHOWCASE_DATA_FILE}"
      COMMAND bash -c "./filtfilt ${FILTFILT_DATA_FILE}"
      COMMAND bash -c "python3 ${PROJECT_SOURCE_DIR}/src/bin/plot.py ${SHOWCASE_DATA_FILE} ${SHOWCASE_PNG_FIGURE}"
      COMMAND bash -c "python3 ${PROJECT_SOURCE_DIR}/src/bin/plot.py ${SHOWCASE_DATA_FILE} ${SHOWCASE_PDF_FIGURE}"
      COMMAND bash -c "python3 ${PROJECT_SOURCE_DIR}/src/bin/plot.py ${FILTFILT_DATA_FILE} ${FILTFILT_PNG_FIGURE}"
      COMMAND bash -c "python3 ${PROJECT_SOURCE_DIR}/src/bin/plot.py ${FILTFILT_DATA_FILE} ${FILTFILT_PDF_FIGURE}"
      COMMAND bash -c "mv ${SHOWCASE_PNG_FIGURE} ${CMAKE_CURRENT_SOURCE_DIR}/imgs"
      COMMAND bash -c "mv ${SHOWCASE_PDF_FIGURE} ${CMAKE_CURRENT_SOURCE_DIR}/imgs"
      COMMAND bash -c "mv ${FILTFILT_PNG_FIGURE} ${CMAKE_CURRENT_SOURCE_DIR}/imgs"
      COMMAND bash -c "mv ${FILTFILT_PDF_FIGURE} ${CMAKE_CURRENT_SOURCE_DIR}/imgs"
      COMMAND bash -c "rm ${SHOWCASE_DATA_FILE}"
      COMMAND bash -c "rm ${FILTFILT_DATA_FILE}"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      COMMENT "Generating figures for documentation"
    )
    add_dependencies(generate_figures showcase filtfilt)
    # Perform substitution of variables in the doxy-file.
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    # Create the "doc" target which will call Doxygen.
    add_custom_target(
      doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
    )
    add_dependencies(doc generate_figures)
    # allow to compile the latex manual
    if(${DOXYGEN_MAKE_LATEX})
      set(DOXYGEN_LATEX_LOG_FILE ${CMAKE_CURRENT_BINARY_DIR}/doc-latex.log)
      add_custom_target(
        doc-latex ALL
        COMMAND make > ${DOXYGEN_LATEX_LOG_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/latex
        COMMENT "Generating API manual using LaTeX"
        VERBATIM
      )
      add_dependencies(doc-latex doc)
    endif(${DOXYGEN_MAKE_LATEX})
  else(${Doxygen_FOUND})
    message(FATAL_ERROR "Doxygen not found, documentation cannot be generated")
  endif(${Doxygen_FOUND})
else(${BUILD_DOC})
  message(STATUS "Doxygen documentation is DISABLED, you can enable it using -DBUILD_DOC=ON (provided that you have Doxygen installed)")
endif(${BUILD_DOC})
