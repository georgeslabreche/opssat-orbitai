# OrbitAI - IPK file generation
This readme will guide you on how to generate a ZIP file and then an IPK file to install the experiment on the OPS-SAT experimenter platform.

## Requirements
1. Make sure you environment is setup to build the MochiMochi component for the spacecraft (ARM target) by following its [README](../Mochi/README.md).
2. Install the NMF space app on your machine by following the `Installation` section of its [ README](../nmf/space-app/README.md).

## Create the ZIP file
1. Configure the NMF space app as want by modifiyng the [configuration file](../nmf/space-app/conf/orbit_ai.properties).
2. Package the NMF space app for OPS-SAT following the official [packaging guide](https://nanosat-mo-framework.readthedocs.io/en/latest/opssat/packaging.html) up to step 9 included.
    1. Note the full path of the `target/nmf-opssat-VERSION/experimenter-package/` folder generated in step 9. We will call it `PACKAGE_PATH`
3. Call the ZIP file creation script with `PACKAGE_PATH` as only argument:
```
./create_ipk_zip.sh [PACKAGE_PATH]
```
The ZIP file is generated in the same directory as the creation script.

## Get the IPK file
1. Send the ZIP to the OPS-SAT DRS following steps 3.1 and 3.2 of this [guide](https://opssat1.esoc.esa.int/dmsf/files/34/view). We already have credentials, ask the team for them.