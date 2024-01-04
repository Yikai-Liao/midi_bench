#include <iostream>
#include "midifile/include/MidiFile.h"
#include "nanobench.h"
#include "symusic/include/symusic.h"
#include "symusic/3rdparty/minimidi/include/minimidi/MiniMidi.hpp"

// filename for cli
int main(int argc, char** argv) {
    if (argc > 1) {
        std::cout << "Parsing " << argv[1] << std::endl;
        constexpr auto loop = 1;
        // set minEpochIterations to 10 to get more stable results
        ankerl::nanobench::Bench().minEpochIterations(50).
        run(
            "minimidi [event level]",
            [&] {
                for(auto i=0; i<loop; ++i) {
                    ankerl::nanobench::doNotOptimizeAway(
                        minimidi::file::MidiFile::from_file(argv[1])
                    );
                }
            }
        ).
        run(
            "symusic  [note level]",
            [&] {
                using namespace symusic;
                for(auto i=0; i<loop; ++i) {
                    ankerl::nanobench::doNotOptimizeAway(Score<Tick>::parse<DataFormat::MIDI>(
                        read_file(std::string(argv[1]))
                    ));
                }
            }
        ).
        run(
            "midifile [event level]",
            [&] {
                for(auto i=0; i<loop; ++i) {
                    smf::MidiFile midifile;
                    midifile.read(argv[1]);
                    midifile.doTimeAnalysis();
                    ankerl::nanobench::doNotOptimizeAway(midifile);
                }
            }
        ).
        run("midifile note level", [&] {
                for(auto i=0; i<loop; ++i) {
                    smf::MidiFile midifile;
                    midifile.read(argv[1]);
                    midifile.doTimeAnalysis();
                    midifile.linkNotePairs();
                    ankerl::nanobench::doNotOptimizeAway(midifile);
                }
            }
        );

    } else {
        std::cout << "Usage : " << argv[0] << " <filename>" << std::endl;
    }
    return 0;
}
