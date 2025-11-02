window.BENCHMARK_DATA = {
  "lastUpdate": 1762072203726,
  "repoUrl": "https://github.com/wsollers/utf_strings",
  "entries": {
    "Benchmark": [
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "87a16fbac55ef5ab2d14b1097210d61100a7100b",
          "message": "Fix GitHub Actions permissions for gh-pages branch creation\n\nAdd required permissions for performance tracking job:\n- contents: write - Allow pushing commits and branches\n- pages: write - Allow GitHub Pages operations\n- id-token: write - Allow OIDC token generation\n\nConfigure git authentication:\n- Use GITHUB_TOKEN for authenticated git operations\n- Set remote URL with token for push permissions\n- Add fetch-depth: 0 for full git history access\n\nFixes 'Permission denied' error when creating gh-pages branch.",
          "timestamp": "2025-11-02T01:31:42-04:00",
          "tree_id": "637b185aea87b450a9fe75417e37b2a25733d39a",
          "url": "https://github.com/wsollers/utf_strings/commit/87a16fbac55ef5ab2d14b1097210d61100a7100b"
        },
        "date": 1762061742077,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7908.726494644839,
            "unit": "ns/iter",
            "extra": "iterations: 173854\ncpu: 7908.1689060936205 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "634889774603a6e65721e70f499edac756882307",
          "message": "Correct Windows preset to conan-default based on CI feedback\n\n- Change all Windows jobs back to conan-default preset\n- CI error showed 'conan-default' as the only available preset, not 'conan-release'\n- Apply to regular MSVC job, sanitizer job, and format-check\n- The actual preset name depends on Conan configuration and build type\n\nCI explicitly reported: 'Available configure presets: conan-default'",
          "timestamp": "2025-11-02T01:38:34-04:00",
          "tree_id": "534fc8e738a8c35b0867e9f8796bfcbb6dfe84b6",
          "url": "https://github.com/wsollers/utf_strings/commit/634889774603a6e65721e70f499edac756882307"
        },
        "date": 1762062167208,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7915.870212404341,
            "unit": "ns/iter",
            "extra": "iterations: 176550\ncpu: 7914.976930048147 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "ba56de960722fd934ba834e0b1f641633d619d89",
          "message": "Windows workflows still going in circles",
          "timestamp": "2025-11-02T01:46:55-04:00",
          "tree_id": "76a4a8526a2613a34fc3f652c06ee2bd5deb6a19",
          "url": "https://github.com/wsollers/utf_strings/commit/ba56de960722fd934ba834e0b1f641633d619d89"
        },
        "date": 1762062591913,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 10462.52818013846,
            "unit": "ns/iter",
            "extra": "iterations: 134208\ncpu: 10461.982385550786 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "fb5bfa9909c25c1ef0af50ac642c7b304ba4f6b2",
          "message": "Fix Windows build presets and test result publishing\n\nWindows preset fixes:\n- Use conan-default for configure (what's available)\n- Use conan-release for build (what's available)\n- Mixed preset approach based on actual CI preset availability\n\nTest publishing fix:\n- Change comment_mode from invalid 'create new' to 'always'\n- Valid options: off, always, changes, failures, errors, etc.\n\nBoth fixes address actual CI runtime errors.",
          "timestamp": "2025-11-02T01:48:17-04:00",
          "tree_id": "599865b7c8b142269b7b93433c5efab8048e0550",
          "url": "https://github.com/wsollers/utf_strings/commit/fb5bfa9909c25c1ef0af50ac642c7b304ba4f6b2"
        },
        "date": 1762062644696,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7909.700005084086,
            "unit": "ns/iter",
            "extra": "iterations: 177023\ncpu: 7909.131361461504 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "2c8fc4d8812e5b7815cbd80db5a354e6941f2b0f",
          "message": "Fix permissions for test result publishing job\n\nAdd required permissions for publish-test-results job:\n- contents: read - Access repository content\n- issues: read - Read issue information\n- checks: write - Create check runs for test results\n- pull-requests: write - Comment on pull requests\n\nFixes 403 Forbidden error: 'Resource not accessible by integration'\nwhen trying to create check runs for test result publishing.",
          "timestamp": "2025-11-02T01:00:24-05:00",
          "tree_id": "0dffa667610dfe5a789a8a887b5dfa7bb2912d0b",
          "url": "https://github.com/wsollers/utf_strings/commit/2c8fc4d8812e5b7815cbd80db5a354e6941f2b0f"
        },
        "date": 1762063401004,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7905.403054035277,
            "unit": "ns/iter",
            "extra": "iterations: 173148\ncpu: 7904.647330607341 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "25860aba1ba05723a729d59c781a967b4db6eedb",
          "message": "Fix Windows MSVC build issues\n\n- Replace MSVC-incompatible Unicode character literals with \\U escape sequences in tests\n- Add pragma to suppress C4251 DLL export warnings for STL types\n- Add explicit template instantiations to resolve linker errors for template library\n- Remove /NXCOMPAT and /DYNAMICBASE from compiler flags (they are linker flags)\n\nFixes:\n- C2015: too many characters in constant (Unicode literals)\n- C4251: DLL interface warnings for std::basic_string\n- LNK2019: unresolved external symbols for UTF string functions\n- D9002: ignoring unknown option '/NXCOMPAT' warnings",
          "timestamp": "2025-11-02T01:06:21-05:00",
          "tree_id": "0325e85a36000ec2e9db94c97e484d477f2673b6",
          "url": "https://github.com/wsollers/utf_strings/commit/25860aba1ba05723a729d59c781a967b4db6eedb"
        },
        "date": 1762063760056,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7921.8542492493125,
            "unit": "ns/iter",
            "extra": "iterations: 176843\ncpu: 7921.464298841345 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "ecbfd91a1d03c0354fd2def3a807a704736886dc",
          "message": "Fix YAML syntax: Correct indentation of performance tracking steps\n\n- Fix indentation of 'Store Performance Results' step (6 spaces to align with other steps)\n- Fix indentation of 'Commit Performance Analysis Reports' step\n- Ensure proper YAML block structure within the performance-tracking job\n\nResolves: YAML syntax error 'expected <block end>, but found -' on line 1012",
          "timestamp": "2025-11-02T01:49:48-05:00",
          "tree_id": "ae9032d36659d4076d13e9b658de0e318a199842",
          "url": "https://github.com/wsollers/utf_strings/commit/ecbfd91a1d03c0354fd2def3a807a704736886dc"
        },
        "date": 1762066289572,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7323.90452388975,
            "unit": "ns/iter",
            "extra": "iterations: 192865\ncpu: 7323.020988774531 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "349ea60e0050746375061614e2106bf5a589be5c",
          "message": "Clean up trailing whitespace in performance index.html\n\n- Remove trailing spaces from JavaScript section\n- Improve code formatting consistency\n- No functional changes to flame graph detection logic",
          "timestamp": "2025-11-02T02:07:07-05:00",
          "tree_id": "b2d63206aaf8bc3a90c62c97154706140d3ee7fa",
          "url": "https://github.com/wsollers/utf_strings/commit/349ea60e0050746375061614e2106bf5a589be5c"
        },
        "date": 1762067347805,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7265.024916253473,
            "unit": "ns/iter",
            "extra": "iterations: 190157\ncpu: 7264.39909653602 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "acf2bb5a866b98544da312a72b8ade79407a236c",
          "message": "Merge remote-tracking branch 'origin/main'",
          "timestamp": "2025-11-02T02:19:04-05:00",
          "tree_id": "be44d99980fbc562dea1f94e87e522d6acad2a9a",
          "url": "https://github.com/wsollers/utf_strings/commit/acf2bb5a866b98544da312a72b8ade79407a236c"
        },
        "date": 1762068063535,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7276.231446864887,
            "unit": "ns/iter",
            "extra": "iterations: 182422\ncpu: 7275.071871813707 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "465e023b334db7ace1898c4d250b3a2034574e1b",
          "message": "fix workflows for windows clang",
          "timestamp": "2025-11-02T02:22:34-05:00",
          "tree_id": "98dae5d53a67d40b430d3b55d10cff2edce35c20",
          "url": "https://github.com/wsollers/utf_strings/commit/465e023b334db7ace1898c4d250b3a2034574e1b"
        },
        "date": 1762068301120,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7271.629983353692,
            "unit": "ns/iter",
            "extra": "iterations: 189832\ncpu: 7270.7823865312475 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "f9db18583196b8d52bfd68ac7317e53c27817245",
          "message": "Fix Windows CMake presets - configure always uses conan-default\n\n- Windows configure step always uses 'conan-default' preset regardless of build type\n- Windows build step uses build-type specific presets (conan-debug/conan-release)\n- Matches the working pattern from Windows AddressSanitizer job\n- Fixes 'No such preset conan-release' error in Windows matrix builds\n- Different from Linux which uses build-type specific configure presets",
          "timestamp": "2025-11-02T02:32:59-05:00",
          "tree_id": "e1722a1bd40bf276475048fea73c513e423b9fa1",
          "url": "https://github.com/wsollers/utf_strings/commit/f9db18583196b8d52bfd68ac7317e53c27817245"
        },
        "date": 1762068930427,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7538.340416212653,
            "unit": "ns/iter",
            "extra": "iterations: 182647\ncpu: 7537.073803566441 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "2f21c252426efbd9096ce77476586e69f915ec10",
          "message": "Merge remote-tracking branch 'origin/main'",
          "timestamp": "2025-11-02T02:38:25-05:00",
          "tree_id": "86b1d17c56463521cc1b2be1ac130218daf16262",
          "url": "https://github.com/wsollers/utf_strings/commit/2f21c252426efbd9096ce77476586e69f915ec10"
        },
        "date": 1762069275477,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7281.589835528142,
            "unit": "ns/iter",
            "extra": "iterations: 189090\ncpu: 7280.847226188589 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "5ce9a1f770fd1e880839641702ccedc9de8b2f9a",
          "message": "Add comprehensive badge collection to README\n\nCategories added:\n🔄 CI/CD Status: All workflow statuses (CI/CD, CodeQL, Fuzz, Release)\n🛠️ Technology Stack: C++23, CMake 3.25+, Conan 2.0+, License\n🖥️ Platform Support: Linux (GCC/Clang), Windows (MSVC/Clang-CL), macOS (Clang)\n🎯 Quality Metrics: Performance benchmarks, security hardening, code quality\n\nFeatures:\n- Live status indicators for all GitHub Actions workflows\n- Technology compatibility and version requirements\n- Cross-platform compiler support matrix\n- Direct links to performance dashboard and security analysis\n- Professional color scheme with appropriate logos\n- Consistent flat style for clean appearance",
          "timestamp": "2025-11-02T02:47:38-05:00",
          "tree_id": "3973d5f36b379bad3e294640ae1ff8e3a816956a",
          "url": "https://github.com/wsollers/utf_strings/commit/5ce9a1f770fd1e880839641702ccedc9de8b2f9a"
        },
        "date": 1762069883881,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7583.870850387072,
            "unit": "ns/iter",
            "extra": "iterations: 184716\ncpu: 7583.522326165573 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "131fc065ad87d229960e70c0a709122ccfe2b47b",
          "message": "Merge remote-tracking branch 'origin/main'",
          "timestamp": "2025-11-02T02:52:56-05:00",
          "tree_id": "1a975f6661fd3a46a46a8ce8839053e5bd2645d8",
          "url": "https://github.com/wsollers/utf_strings/commit/131fc065ad87d229960e70c0a709122ccfe2b47b"
        },
        "date": 1762070110330,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7401.280336207924,
            "unit": "ns/iter",
            "extra": "iterations: 192262\ncpu: 7400.36684836317 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "fddfca9ded95987725f329b1bcc14f30097cc01b",
          "message": "fix windows again",
          "timestamp": "2025-11-02T02:58:00-05:00",
          "tree_id": "630d2779bf94e3a9765b6e842a1b18382f70daa1",
          "url": "https://github.com/wsollers/utf_strings/commit/fddfca9ded95987725f329b1bcc14f30097cc01b"
        },
        "date": 1762070446843,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7290.099673272256,
            "unit": "ns/iter",
            "extra": "iterations: 187312\ncpu: 7289.718293542325 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "071a0a241ca5fc72e3ac0f49e288bc9845463ff0",
          "message": "Fix Windows CMake generator configuration\n\n- Override generator to use 'Visual Studio 17 2022' for all Windows builds\n- Prevents MinGW Makefiles generator from being used on Windows\n- Fixes Clang-CL and MSVC builds that were failing with generator/platform mismatch\n- Both windows-msvc matrix job and windows-msvc-debug job updated",
          "timestamp": "2025-11-02T03:06:28-05:00",
          "tree_id": "7d30b5fbfab06f35bcee32de7b4ea480f30d9558",
          "url": "https://github.com/wsollers/utf_strings/commit/071a0a241ca5fc72e3ac0f49e288bc9845463ff0"
        },
        "date": 1762070962644,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7259.678947917883,
            "unit": "ns/iter",
            "extra": "iterations: 191620\ncpu: 7258.648361340152 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "1bd5f2c191725fb4565fd3aa1e934437fa2ee4fc",
          "message": "Fix MSVC /HIGHENTROPYVA flag placement\n\n- Remove /HIGHENTROPYVA from compiler flags (SECURITY_FLAGS)\n- Keep /HIGHENTROPYVA only in linker flags where it belongs\n- Fixes Windows build error: 'invalid numeric argument /HIGHENTROPYVA'\n- /HIGHENTROPYVA is a linker-only flag for Windows ASLR security",
          "timestamp": "2025-11-02T03:11:16-05:00",
          "tree_id": "a8348e0a866e0912223a410a69d0134a5fddf0d9",
          "url": "https://github.com/wsollers/utf_strings/commit/1bd5f2c191725fb4565fd3aa1e934437fa2ee4fc"
        },
        "date": 1762071169534,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7252.424406860641,
            "unit": "ns/iter",
            "extra": "iterations: 192577\ncpu: 7252.036691816781 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "69cebec2dcf6beec598fcb20a10555000048f5f4",
          "message": "Fix Clang-CL compiler compatibility issues\n\n- Add IS_CLANG_CL detection to differentiate Clang-CL from MSVC\n- Remove MSVC-specific flags (/LTCG, /GL) when using Clang-CL\n- Clang-CL uses different LTO mechanisms than MSVC\n- Prevents 'ignoring unknown option /LTCG' warnings\n- Maintains MSVC optimization flags for actual MSVC builds\n- Fixes Windows Clang-CL matrix build compatibility",
          "timestamp": "2025-11-02T03:16:39-05:00",
          "tree_id": "3dbcafd42fba2be27b113c625cfbd295d73688c7",
          "url": "https://github.com/wsollers/utf_strings/commit/69cebec2dcf6beec598fcb20a10555000048f5f4"
        },
        "date": 1762071625582,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7347.9812939330495,
            "unit": "ns/iter",
            "extra": "iterations: 189778\ncpu: 7347.335729115072 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "3a10f3827471a25f0badc6210f404577b94e51b1",
          "message": "Comprehensive Clang-CL build fixes\n\n- Add FORCE_CLANG_CL flag from CI to explicitly identify Clang-CL builds\n- Clean build directories before each Windows matrix job to prevent conflicts\n- Enhanced detection with explicit CI flag as primary method\n- Improved debug output to track compiler identification\n- Should resolve LTCG flag issues and shared library export problems\n- Clean separation between MSVC and Clang-CL build configurations",
          "timestamp": "2025-11-02T03:22:50-05:00",
          "tree_id": "1f4c1a8416897e2d47b859b1c3d2c5b76d61f415",
          "url": "https://github.com/wsollers/utf_strings/commit/3a10f3827471a25f0badc6210f404577b94e51b1"
        },
        "date": 1762071954082,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7314.752576997171,
            "unit": "ns/iter",
            "extra": "iterations: 191890\ncpu: 7313.753384751681 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "committer": {
            "email": "wsollers@gmail.com",
            "name": "BoondockTaints",
            "username": "wsollers"
          },
          "distinct": true,
          "id": "b10bcf0379c0e10a15e8ebc0a8148829742ece09",
          "message": "Implement clean external flag approach for compiler-specific configuration\n\n- Replace complex detection logic with simple USE_MSVC_LTO boolean flag\n- MSVC builds: USE_MSVC_LTO=ON enables /LTCG and /GL flags\n- Clang-CL builds: USE_MSVC_LTO=OFF disables MSVC-specific flags\n- ENABLE_SHARED_LIBRARY flag controls shared library building per compiler\n- Cleaner separation of concerns between CI and CMake\n- Eliminates compiler detection ambiguity and cache conflicts\n- More maintainable and explicit configuration approach",
          "timestamp": "2025-11-02T03:28:16-05:00",
          "tree_id": "eca74af6087bf3e227425a67ae3aa0b70eb8385b",
          "url": "https://github.com/wsollers/utf_strings/commit/b10bcf0379c0e10a15e8ebc0a8148829742ece09"
        },
        "date": 1762072203354,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7260.439798196922,
            "unit": "ns/iter",
            "extra": "iterations: 191672\ncpu: 7258.401680996702 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}