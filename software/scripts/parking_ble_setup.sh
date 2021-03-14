
# Setup for ParkingBLE project
# Run script from "software" directory (./scripts/parking_ble_setup.sh)

proj=./apps/project_firmware

# Step 1: Update submodule
git submodule update --init --recursive ;

# Step 2: Install web server materials
pushd ./ ; cd ${proj}/web && npm install ; popd ;

# Step 3: Configure system setup
pushd ./ ; cd ${proj} && ./setup/configure ; popd ;

