cmake_minimum_required(VERSION 3.9)

project (Topper.Plugins)

set (PLUGINS
    Helmet.Enterprise.TCPProtocol
    Topper.Exchange.Kraken
    Topper.Strategy.Threshold
    Topper.Transaction.Blockchain
    Topper.Workbench
)

foreach (SUBDIR ${PLUGINS})
    add_subdirectory(${SUBDIR})
endforeach ()