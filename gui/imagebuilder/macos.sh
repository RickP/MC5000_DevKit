#!/bin/bash

cd ..
mkdir images
mkdir build
cd build
qmake ..
make
echo "--> Start packaging process"
macdeployqt MC5000-GUI.app -qmldir=..

if [[ "${APPLE_DEV_IDENTITY}" == "" ]]; then
	echo "WARNING: No credentials for signing found"
	echo "WARNING: dmg package won't be signed and notarized"
  echo "--> Creating dmg"
  hdiutil create -srcfolder MC5000-GUI.app MC5000-GUI.dmg
  mv MC5000-GUI.dmg ../images/MC5000-GUI_MacOS-x86_64-unsigned.dmg
else
  echo "--> Signing app bundle"
  codesign -vvv --deep --strict --options=runtime --sign ${APPLE_DEV_IDENTITY} MC5000-GUI.app
  echo "--> Creating dmg"
  hdiutil create -srcfolder MC5000-GUI.app MC5000-GUI.dmg
  echo "--> Signing dmg"
  codesign -vvv --strict --options=runtime --sign ${APPLE_DEV_IDENTITY} MC5000-GUI.dmg
	response=$(xcrun altool -t osx -f MC5000-GUI.dmg --primary-bundle-id "org.rickp.mc5000" --notarize-app -u "${APPLE_DEV_USER}" -p "${APPLE_DEV_PASS}")
	requestUUID=$(echo "${response}" | tr ' ' '\n' | tail -1)
	for ((ATTEMPT=5; ATTEMPT>=1; ATTEMPT--))
	do
	  echo "--> Checking notarization status"
	  statusCheckResponse=$(xcrun altool --notarization-info "${requestUUID}" -u "${APPLE_DEV_USER}" -p "${APPLE_DEV_PASS}")

	  isSuccess=$(echo "${statusCheckResponse}" | grep "success")
	  isFailure=$(echo "${statusCheckResponse}" | grep "invalid")

	  if [[ "${isSuccess}" != "" ]]; then
	    echo "Notarization done!"
	    xcrun stapler staple "MC5000-GUI.dmg"
	    EXIT_CODE=$?
	    if [ ${EXIT_CODE} -ne 0 ]; then
	      echo "Stapler failed!"
	      exit ${EXIT_CODE}
	    fi
	    echo "Stapler done!"
      mv MC5000-GUI.dmg ../images/MC5000-GUI_MacOS-x86_64.dmg
	    break
	  fi
	  if [[ "${isFailure}" != "" ]]; then
	    echo "${statusCheckResponse}"
	    echo "Notarization failed"
      break
	  fi

	  echo "Notarization not finished yet, sleep 2m then check again..."
	  for num in {1..12}
	  do
	    sleep 10
	    echo "Elapsed: ${num}0 sec"
	  done
	done
fi

cd ..
rm -r build
