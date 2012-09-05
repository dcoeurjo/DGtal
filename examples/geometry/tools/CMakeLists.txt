
IF(WITH_VISU3D_QGLVIEWER)
  
  SET(DGTAL_EXAMPLES_SRC
    volWatershed
    )
  
  
  FOREACH(FILE ${DGTAL_EXAMPLES_SRC})
    add_executable(${FILE} ${FILE})
    target_link_libraries (${FILE} DGtal DGtalIO )
  ENDFOREACH(FILE)
  
ENDIF()

