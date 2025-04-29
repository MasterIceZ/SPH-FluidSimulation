#!/bin/bash

IMGUI_REPO="imgui"
DEST_DIR="../include/imgui"

set -e

echo "Cloning ImGui repository..."
git clone https://github.com/ocornut/imgui.git

echo "Creating destination directory..."
mkdir -p "$DEST_DIR"

echo "Copying ImGui core files..."
cp "$IMGUI_REPO"/imgui*.h "$DEST_DIR"
cp "$IMGUI_REPO"/imgui*.cpp "$DEST_DIR"
cp "$IMGUI_REPO"/imgui_draw.* "$DEST_DIR"
cp "$IMGUI_REPO"/imconfig.h "$DEST_DIR"
cp "$IMGUI_REPO"/imstb_*.h "$DEST_DIR"

echo "Copying ImGui backends..."
cp "$IMGUI_REPO"/backends/imgui_impl_glfw.* "$DEST_DIR"
cp "$IMGUI_REPO"/backends/imgui_impl_opengl3.* "$DEST_DIR"
cp "$IMGUI_REPO"/backends/imgui_impl_opengl3_loader.* "$DEST_DIR"

echo "Installation Completed!"
echo "ImGui files have been copied to $DEST_DIR"

rm -rf "$IMGUI_REPO"