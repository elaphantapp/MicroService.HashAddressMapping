#!/bin/bash

set -o nounset
set -o errexit

CURRENT_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd);
cd "$CURRENT_DIR";

source "download-urls";
if [ ! -f ".$CONTACT_ZIPNAME" ]; then
    echo "Downloading $CONTACT_ZIPNAME...";
    #echo curl --location "$CONTACT_URL" --output "$CONTACT_ZIPNAME";
    curl --location "$CONTACT_URL" --output "$CONTACT_ZIPNAME";
    unzip -t "$CONTACT_ZIPNAME";
    touch ".$CONTACT_ZIPNAME";
else
    echo "$CONTACT_ZIPNAME exists.";
fi

CONTACT_AARS=("crosspl-lib" "Elastos.SDK.Contact");
for aar in ${CONTACT_AARS[@]}; do
    if [ ! -f ".$aar.unpacked" ]; then
        echo "Unpacking $aar...";
        unzip $CONTACT_ZIPNAME "$aar-*.aar";
        touch ".$aar.unpacked";
    else
        echo "$aar exists.";
    fi
done

echo "Ensure depend aars done!"
