# HIRA Data Analysis

This script, `hira_data_analysis.C`, plots all necessary 1D and 2D histograms required for data analysis, with a unique real-time graphical cut feature in **TBrowser**.

## Features

- **1D and 2D Histograms**: Generates both 1D and 2D histograms to visualize key data metrics.
- **Graphical Cuts**: Allows users to place a cut interactively on the histogram within **TBrowser**.
- **Real-Time Updates**: As cut points are adjusted, the legend automatically updates to reflect the number of data points or area within the selected cut region.

## Getting Started

1. Ensure **CERN ROOT** is installed.
2. Load and run the script in ROOT.

## Usage

1. Run the script in ROOT. The histograms will automatically open in **TBrowser**.
2. Place a graphical cut to highlight the area of interest.
3. The legend will update in real time as you adjust the cut.

## Dependencies

- CERN ROOT framework
