window.BENCHMARK_DATA = {
  "lastUpdate": 1762144118887,
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
          "id": "5373a67b3ea4a6a4d9352d3398290ceaaf2712a3",
          "message": "Fix performance baseline tracking build configuration\n\n- Disable LTO (USE_LTO=OFF) to avoid linking issues with GoogleTest\n- Use libstdc++ instead of libc++ (USE_LIBC_PLUS_PLUS=OFF) for compatibility\n- Update Conan profile to use libstdc++11 matching CMake configuration\n- Keep native architecture optimization (USE_NATIVE_ARCH=ON) for performance\n- Maintain Clang 18 optimizations without problematic LTO complexity\n\nResolves linking errors:\n- undefined reference to testing::internal::PrintTo\n- undefined reference to testing::internal::GetBoolAssertionFailureMessage\n\nPerformance tracking will still be effective with -O3 + -march=native optimizations\nwithout the LTO + mixed standard library complications.",
          "timestamp": "2025-11-02T03:37:35-05:00",
          "tree_id": "3831ab9df7ec116053841b1f5d1ca0c46bf75ff3",
          "url": "https://github.com/wsollers/utf_strings/commit/5373a67b3ea4a6a4d9352d3398290ceaaf2712a3"
        },
        "date": 1762072741376,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7068.725592221692,
            "unit": "ns/iter",
            "extra": "iterations: 198912\ncpu: 7067.812640765767 ns\nthreads: 1"
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
          "id": "0e0214b751c5b08a3f87d1f0b1a37577e946a200",
          "message": "Fix GitHub Pages deployment with proper Actions workflow\n\n- Add explicit Pages deployment using actions/deploy-pages@v4\n- Ensure .nojekyll file exists to disable Jekyll processing\n- Add proper permissions for Pages deployment (pages: write, id-token: write)\n- Upload docs/ directory as static Pages artifact instead of relying on Jekyll\n\nThis should resolve the Jekyll 'No such file or directory' error by using\nGitHub Actions for Pages deployment instead of Jekyll processing.",
          "timestamp": "2025-11-02T03:52:12-05:00",
          "tree_id": "b125c1690732f2263276e3abf5a84664fada26a9",
          "url": "https://github.com/wsollers/utf_strings/commit/0e0214b751c5b08a3f87d1f0b1a37577e946a200"
        },
        "date": 1762073673797,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7034.730546889551,
            "unit": "ns/iter",
            "extra": "iterations: 198413\ncpu: 7033.743086390508 ns\nthreads: 1"
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
          "id": "9883846252de7128aac3647f8abf43d3a74c53db",
          "message": "Merge branch 'main' of https://github.com/wsollers/utf_strings",
          "timestamp": "2025-11-02T03:56:44-05:00",
          "tree_id": "1e32e15051504c457ec1a4f373874bf3e10819cc",
          "url": "https://github.com/wsollers/utf_strings/commit/9883846252de7128aac3647f8abf43d3a74c53db"
        },
        "date": 1762074242262,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7015.386658389932,
            "unit": "ns/iter",
            "extra": "iterations: 194924\ncpu: 7014.471322156326 ns\nthreads: 1"
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
          "id": "9883846252de7128aac3647f8abf43d3a74c53db",
          "message": "Merge branch 'main' of https://github.com/wsollers/utf_strings",
          "timestamp": "2025-11-02T03:56:44-05:00",
          "tree_id": "1e32e15051504c457ec1a4f373874bf3e10819cc",
          "url": "https://github.com/wsollers/utf_strings/commit/9883846252de7128aac3647f8abf43d3a74c53db"
        },
        "date": 1762074387154,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7024.167892986721,
            "unit": "ns/iter",
            "extra": "iterations: 197882\ncpu: 7023.842512204243 ns\nthreads: 1"
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
          "id": "250896e000712558572e6cec3abc98d8c59cb453",
          "message": "Resolve merge conflict: keep performance files deleted\n\nThe performance files (benchmark_console.txt, benchmark_results.csv,\ndetailed_benchmark.json, system_info.txt) should be auto-generated by CI\nrather than committed to git, so confirming their deletion as intended.",
          "timestamp": "2025-11-02T04:13:25-05:00",
          "tree_id": "d049db9d222e147410b13f6fd14181306670669b",
          "url": "https://github.com/wsollers/utf_strings/commit/250896e000712558572e6cec3abc98d8c59cb453"
        },
        "date": 1762074913713,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7095.001677227003,
            "unit": "ns/iter",
            "extra": "iterations: 198542\ncpu: 7094.115773992405 ns\nthreads: 1"
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
          "id": "5d071248220072a0fc29db8268ad1fa6f723966d",
          "message": "Fix GitHub Pages deployment with proper job separation\n\nIssues fixed:\n- Move Pages deployment to dedicated job with proper environment setup\n- Add required 'github-pages' environment for deployment\n- Ensure job runs after performance-tracking completion\n- Use proper permissions (pages: write, id-token: write)\n- Remove duplicate configure-pages setup from performance job\n\nThis should resolve the Jekyll build errors by ensuring GitHub Actions\ndeployment runs properly and takes precedence over legacy Jekyll builds.",
          "timestamp": "2025-11-02T04:16:51-05:00",
          "tree_id": "df71b2db18887de97a62304a8a3d54a15e121655",
          "url": "https://github.com/wsollers/utf_strings/commit/5d071248220072a0fc29db8268ad1fa6f723966d"
        },
        "date": 1762075192721,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7030.7052408501795,
            "unit": "ns/iter",
            "extra": "iterations: 193938\ncpu: 7030.145170105909 ns\nthreads: 1"
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
          "id": "02b335a2f081e6445fd62c4227a22fa702d003b7",
          "message": "Add Cppcheck static analysis workflow with SARIF upload\n\nFeatures:\n- Comprehensive C++23 static analysis with Cppcheck\n- SARIF format output for GitHub Code Scanning integration\n- Configurable severity levels (all, error, warning, style, performance, portability, information)\n- Daily scheduled analysis at 3 AM UTC\n- XML and human-readable report generation\n- Automatic issue creation for critical findings on scheduled runs\n- PR comments with analysis results\n- Platform-specific configuration (Unix 64-bit, C++23 standard)\n- Comprehensive suppressions for common false positives\n- Artifact upload for detailed review\n\nThis adds another layer of static analysis beyond CodeQL and Semgrep,\nspecifically focused on C++ code quality, performance, and portability issues.",
          "timestamp": "2025-11-02T04:19:25-05:00",
          "tree_id": "ac21023758bad2c365a05d15f38d0369c23e09f5",
          "url": "https://github.com/wsollers/utf_strings/commit/02b335a2f081e6445fd62c4227a22fa702d003b7"
        },
        "date": 1762075287438,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7023.970826083015,
            "unit": "ns/iter",
            "extra": "iterations: 198842\ncpu: 7022.139844700817 ns\nthreads: 1"
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
          "id": "bcf516e336f7e81bac2d534e601e6876babb27da",
          "message": "Fix Pages deployment by properly separating jobs and artifacts\n\nIssues fixed:\n- Move Pages artifact upload from performance-tracking to deploy-pages job\n- Add artifact transfer between jobs (docs-for-pages artifact)\n- Ensure deploy-pages job downloads docs content before deployment\n- Maintain proper job separation while enabling artifact access\n- Fix 'Missing environment' error by keeping environment in deploy-pages job\n\nThis resolves the GitHub Pages deployment error where the deploy-pages job\ncouldn't access the docs content generated by performance-tracking job.",
          "timestamp": "2025-11-02T04:23:09-05:00",
          "tree_id": "dea03d8ac59f6eff0b29f9e65ed0efbed841f7fb",
          "url": "https://github.com/wsollers/utf_strings/commit/bcf516e336f7e81bac2d534e601e6876babb27da"
        },
        "date": 1762075502390,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7088.214226791154,
            "unit": "ns/iter",
            "extra": "iterations: 198808\ncpu: 7087.476806768339 ns\nthreads: 1"
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
          "id": "d00ce48fd756d3a59e5d8e04f11565cb03745883",
          "message": "feat: Add comprehensive SAST security scanning workflow\n\n- Create sast-scanning.yml workflow integrating multiple security tools\n- Add Trivy vulnerability and misconfiguration scanning with SARIF upload\n- Add Checkov infrastructure security analysis for GitHub Actions/Docker\n- Add Gitleaks secret detection across full git history\n- Integrate existing Cppcheck and Semgrep workflows via workflow dispatch\n- Generate comprehensive security reports with PR comments and issue creation\n- Add customized .gitleaks.toml configuration with project-specific rules\n- Update README with new SAST security scanning badge\n- Update .gitignore to exclude scan result files\n\nSecurity Tools Coverage:\n- Trivy: Vulnerability & misconfiguration scanning\n- Checkov: Infrastructure-as-Code security analysis\n- Gitleaks: Secret detection in git history\n- Cppcheck: Static code analysis (existing workflow)\n- Semgrep: Security pattern matching (existing workflow)\n\nAll tools upload SARIF results to GitHub Security tab for unified view.\nDaily scheduled scans create issues for critical findings.\nConfigurable scan intensity levels (basic/comprehensive/deep).",
          "timestamp": "2025-11-02T04:28:03-05:00",
          "tree_id": "afcad4980a6b3ca321b9f10e14d4b598c97b7d80",
          "url": "https://github.com/wsollers/utf_strings/commit/d00ce48fd756d3a59e5d8e04f11565cb03745883"
        },
        "date": 1762075860789,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 6799.809390171721,
            "unit": "ns/iter",
            "extra": "iterations: 204533\ncpu: 6798.711562437357 ns\nthreads: 1"
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
          "id": "7dfa940021992598654a28464231a5783e356954",
          "message": "Fix workflows",
          "timestamp": "2025-11-02T04:31:02-05:00",
          "tree_id": "ce39bd08193d2e21baffabf415525eb96a066d25",
          "url": "https://github.com/wsollers/utf_strings/commit/7dfa940021992598654a28464231a5783e356954"
        },
        "date": 1762076039525,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7021.955465048433,
            "unit": "ns/iter",
            "extra": "iterations: 195442\ncpu: 7021.3204787097975 ns\nthreads: 1"
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
          "id": "49fa16888048bb956690b8020c994ebd4cebf336",
          "message": "ensure well formed",
          "timestamp": "2025-11-02T04:33:43-05:00",
          "tree_id": "aee99fde2922fc2b7a517f65b57e928d1f356c29",
          "url": "https://github.com/wsollers/utf_strings/commit/49fa16888048bb956690b8020c994ebd4cebf336"
        },
        "date": 1762076146083,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7027.72814133143,
            "unit": "ns/iter",
            "extra": "iterations: 201682\ncpu: 7026.761882567606 ns\nthreads: 1"
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
          "id": "46c1474bd82af3b9d6f82fed78bb55c8ea854b88",
          "message": "security workflow strteamline",
          "timestamp": "2025-11-02T04:43:41-05:00",
          "tree_id": "bc81d3913b5c798f37686896e80344ff0b1937af",
          "url": "https://github.com/wsollers/utf_strings/commit/46c1474bd82af3b9d6f82fed78bb55c8ea854b88"
        },
        "date": 1762076811819,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7078.83752748472,
            "unit": "ns/iter",
            "extra": "iterations: 198292\ncpu: 7078.466983035119 ns\nthreads: 1"
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
          "id": "6dfd0d490212c5e0f65bd36f6a895863a29601e9",
          "message": "fix: Resolve SAST scanning issues and improve reliability\n\n- Fix invalid .gitleaks.toml configuration syntax\n- Add comprehensive error handling for Gitleaks with fallback scanning\n- Replace non-existent cppcheck-sarif package with custom XML-to-SARIF converter\n- Enhance GitHub Pages deployment with better Jekyll disable handling\n- Remove dependency issues that caused workflow failures\n\nChanges:\n- .gitleaks.toml: Fixed TOML syntax and simplified allowlist configuration\n- sast-scanning.yml: Added robust error recovery and custom SARIF conversion\n- ci.yml: Improved Pages deployment with better .nojekyll handling\n\nAll SARIF results now properly upload to GitHub Security/Code Scanning tab",
          "timestamp": "2025-11-02T04:53:56-05:00",
          "tree_id": "6dd2ce440cb64069583fd84c9b63bc997821b402",
          "url": "https://github.com/wsollers/utf_strings/commit/6dfd0d490212c5e0f65bd36f6a895863a29601e9"
        },
        "date": 1762077347948,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7032.524792577562,
            "unit": "ns/iter",
            "extra": "iterations: 198749\ncpu: 7031.871863506231 ns\nthreads: 1"
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
          "id": "8c83981116650666f39e95d4302b68b140befb1a",
          "message": "fix: Resolve bash arithmetic syntax error in Cppcheck summary\n\n- Add proper validation for grep count results to ensure numeric values\n- Prevent arithmetic expression errors when XML parsing returns empty results\n- Ensure ERROR_COUNT, WARNING_COUNT, and STYLE_COUNT are valid integers before arithmetic",
          "timestamp": "2025-11-02T04:55:34-05:00",
          "tree_id": "c1fdc40a551d429cf15aa4ba3c13519a1620e84a",
          "url": "https://github.com/wsollers/utf_strings/commit/8c83981116650666f39e95d4302b68b140befb1a"
        },
        "date": 1762077467571,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 6990.572899633824,
            "unit": "ns/iter",
            "extra": "iterations: 199370\ncpu: 6989.065250539198 ns\nthreads: 1"
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
          "id": "13c09e96852ee544de9e8456000cd957cd0f4639",
          "message": "ci: Separate GitHub Pages deployment into dedicated workflow\n\n- Removed Pages deployment from main CI workflow (ci.yml)\n- Created dedicated pages-deploy.yml workflow that triggers on CI completion\n- Improved Jekyll disable approach with comprehensive conflict removal\n- Added artifact download from completed CI workflow run\n- Enhanced static site preparation with permission management\n- Updated documentation to reflect the workflow separation\n\nThis separation isolates Pages deployment from CI, preventing Jekyll\nprocessing issues from affecting the main CI pipeline reliability.",
          "timestamp": "2025-11-02T05:03:30-05:00",
          "tree_id": "e723b0206595ecf9957def44932e9b44962dced8",
          "url": "https://github.com/wsollers/utf_strings/commit/13c09e96852ee544de9e8456000cd957cd0f4639"
        },
        "date": 1762078034935,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7028.842807135333,
            "unit": "ns/iter",
            "extra": "iterations: 196542\ncpu: 7028.255492464714 ns\nthreads: 1"
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
          "id": "e5ec6d55d55f378f4fc22c5e0d781d63b8ca20d9",
          "message": "FIx Workflows",
          "timestamp": "2025-11-02T05:04:37-05:00",
          "tree_id": "ff5b966384fc811a22b95ef64212ff6aa42e4442",
          "url": "https://github.com/wsollers/utf_strings/commit/e5ec6d55d55f378f4fc22c5e0d781d63b8ca20d9"
        },
        "date": 1762078128516,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7030.701712676289,
            "unit": "ns/iter",
            "extra": "iterations: 198111\ncpu: 7030.271882934313 ns\nthreads: 1"
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
          "id": "13ad2a555af37c6d4e1182237eac592730105891",
          "message": "ci: Fix unzip interactive prompt in Pages deployment\n\n- Added -o flag to unzip command to overwrite files without prompting\n- Added rm -rf docs/ before extraction to ensure clean state\n- Resolves 'replace docs/README.md? [y]es, [n]o...' interactive prompt error\n\nThis prevents the workflow from hanging on file conflicts during\ndocs artifact extraction.",
          "timestamp": "2025-11-02T05:10:33-05:00",
          "tree_id": "f5154e5305f4a91a0439a142584098cbb80382fc",
          "url": "https://github.com/wsollers/utf_strings/commit/13ad2a555af37c6d4e1182237eac592730105891"
        },
        "date": 1762078381253,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 6831.208170874638,
            "unit": "ns/iter",
            "extra": "iterations: 205437\ncpu: 6830.888160360597 ns\nthreads: 1"
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
          "id": "212fe75423f0584414a62d19027480d186943617",
          "message": "Fix workflow",
          "timestamp": "2025-11-02T05:11:28-05:00",
          "tree_id": "7a72f98e1eee5d32b7b029ebad6a6c99d635797b",
          "url": "https://github.com/wsollers/utf_strings/commit/212fe75423f0584414a62d19027480d186943617"
        },
        "date": 1762078576734,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7046.791050466543,
            "unit": "ns/iter",
            "extra": "iterations: 199340\ncpu: 7045.964091501956 ns\nthreads: 1"
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
          "id": "6a91d5a013ee866c1e0417e7f943f3d6f57b2ada",
          "message": "feat: Prepare for v0.0.1 release\n\n- Update CMakeLists.txt version from 0.1.0 to 0.0.1\n- Add include/utf/version.hpp with semantic version constants\n- Include version.hpp in main utf_strings.hpp header\n- Add comprehensive create-release.yml workflow that:\n  - Builds release and debug binaries for Linux (GCC/Clang) and Windows (MSVC/Clang)\n  - Packages binaries with headers, libraries, and documentation\n  - Creates GitHub releases with downloadable archives\n  - Supports both tag-triggered and manual workflow dispatch\n  - Generates comprehensive changelog and release notes\n\nReady for v0.0.1 tag creation and release!",
          "timestamp": "2025-11-02T10:24:11-05:00",
          "tree_id": "516790873816abff65793e9272cbfe0f159f4d6f",
          "url": "https://github.com/wsollers/utf_strings/commit/6a91d5a013ee866c1e0417e7f943f3d6f57b2ada"
        },
        "date": 1762097236522,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7018.079361321643,
            "unit": "ns/iter",
            "extra": "iterations: 198535\ncpu: 7016.849860226158 ns\nthreads: 1"
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
          "id": "270a7c1d78ed55aa9250b65b3204d8e8a1cc9f7a",
          "message": "docs: Add Create Release workflow documentation\n\n- Added comprehensive documentation for create-release.yml workflow\n- Documented platform matrix (Linux GCC/Clang, Windows MSVC/Clang)\n- Explained release asset structure and contents\n- Added usage examples for tag-based and manual release creation\n- Updated workflow matrix table to include Create Release column\n- Enhanced running workflows section with release creation commands\n\nThe documentation now covers the complete automated release system\nwith cross-platform binary distribution and GitHub release creation.",
          "timestamp": "2025-11-02T10:27:26-05:00",
          "tree_id": "ee148d6abc8ba4e5163aec54a274fb669710aeab",
          "url": "https://github.com/wsollers/utf_strings/commit/270a7c1d78ed55aa9250b65b3204d8e8a1cc9f7a"
        },
        "date": 1762097353010,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 6845.794470090796,
            "unit": "ns/iter",
            "extra": "iterations: 205790\ncpu: 6845.07918752126 ns\nthreads: 1"
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
          "id": "4e611427adf32fa028a264906da4898b8a17ebe1",
          "message": "Fix perf.data file location and processing issues\n\n- Add explicit perf output file paths with -o flag\n- Enhanced debugging to show directory contents and file locations\n- Fix perf script commands to use -i flag for explicit input files\n- Add comprehensive error checking and file validation\n- Clean up perf files in multiple directories to avoid conflicts\n- Add alternative perf recording with different output files",
          "timestamp": "2025-11-02T12:00:02-05:00",
          "tree_id": "24991c72fbfa0cca02b39f03fcce22c3cd878ca9",
          "url": "https://github.com/wsollers/utf_strings/commit/4e611427adf32fa028a264906da4898b8a17ebe1"
        },
        "date": 1762102970740,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7023.4222506536025,
            "unit": "ns/iter",
            "extra": "iterations: 198529\ncpu: 7022.569715255706 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "4e611427adf32fa028a264906da4898b8a17ebe1",
          "message": "Fix perf.data file location and processing issues\n\n- Add explicit perf output file paths with -o flag\n- Enhanced debugging to show directory contents and file locations\n- Fix perf script commands to use -i flag for explicit input files\n- Add comprehensive error checking and file validation\n- Clean up perf files in multiple directories to avoid conflicts\n- Add alternative perf recording with different output files",
          "timestamp": "2025-11-02T17:00:02Z",
          "url": "https://github.com/wsollers/utf_strings/commit/4e611427adf32fa028a264906da4898b8a17ebe1"
        },
        "date": 1762103208689,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7061.487377870907,
            "unit": "ns/iter",
            "extra": "iterations: 196718\ncpu: 7060.531461279598 ns\nthreads: 1"
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
          "id": "2200defa93c33196727bcec9ded8e23e37796af1",
          "message": "Fix GitHub Pages deployment to include Performance Baseline artifacts\n\n- Add Performance Baseline Tracking workflow as trigger for Pages deployment\n- Update artifact download logic to merge CI and Performance artifacts\n- Handle both CI→Performance and Performance→CI workflow sequences\n- Extract and merge documentation from both sources\n- Ensure flame graphs and performance analysis files reach GitHub Pages\n\nThis resolves the issue where flame graphs were generated correctly\nbut missing from the deployed site due to artifact isolation between workflows.",
          "timestamp": "2025-11-02T12:13:56-05:00",
          "tree_id": "74724036fc7721431095527ed10040ff6f00ce78",
          "url": "https://github.com/wsollers/utf_strings/commit/2200defa93c33196727bcec9ded8e23e37796af1"
        },
        "date": 1762103804226,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7056.4247631591315,
            "unit": "ns/iter",
            "extra": "iterations: 197390\ncpu: 7055.935913673438 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "2200defa93c33196727bcec9ded8e23e37796af1",
          "message": "Fix GitHub Pages deployment to include Performance Baseline artifacts\n\n- Add Performance Baseline Tracking workflow as trigger for Pages deployment\n- Update artifact download logic to merge CI and Performance artifacts\n- Handle both CI→Performance and Performance→CI workflow sequences\n- Extract and merge documentation from both sources\n- Ensure flame graphs and performance analysis files reach GitHub Pages\n\nThis resolves the issue where flame graphs were generated correctly\nbut missing from the deployed site due to artifact isolation between workflows.",
          "timestamp": "2025-11-02T17:13:56Z",
          "url": "https://github.com/wsollers/utf_strings/commit/2200defa93c33196727bcec9ded8e23e37796af1"
        },
        "date": 1762104074832,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7025.102370916846,
            "unit": "ns/iter",
            "extra": "iterations: 189800\ncpu: 7024.530384615384 ns\nthreads: 1"
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
          "id": "828c17649b9a5608d760e8a27dd1bf6055cd72df",
          "message": "Remove performance README.md to enable interactive dashboard\n\nThe README.md in docs/performance/ was overriding the dynamic index.html\nperformance dashboard created by the workflow. Jekyll prioritizes README.md\nfor directory routing, so removing it allows the interactive performance\ndashboard with flame graphs and live data to be served at /performance/",
          "timestamp": "2025-11-02T12:21:42-05:00",
          "tree_id": "3a98a318f2a58212c55a57fa6c8c43ffe7473c87",
          "url": "https://github.com/wsollers/utf_strings/commit/828c17649b9a5608d760e8a27dd1bf6055cd72df"
        },
        "date": 1762104233464,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7156.406959874257,
            "unit": "ns/iter",
            "extra": "iterations: 197130\ncpu: 7155.48608025161 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "828c17649b9a5608d760e8a27dd1bf6055cd72df",
          "message": "Remove performance README.md to enable interactive dashboard\n\nThe README.md in docs/performance/ was overriding the dynamic index.html\nperformance dashboard created by the workflow. Jekyll prioritizes README.md\nfor directory routing, so removing it allows the interactive performance\ndashboard with flame graphs and live data to be served at /performance/",
          "timestamp": "2025-11-02T17:21:42Z",
          "url": "https://github.com/wsollers/utf_strings/commit/828c17649b9a5608d760e8a27dd1bf6055cd72df"
        },
        "date": 1762104646910,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7138.423673005774,
            "unit": "ns/iter",
            "extra": "iterations: 198927\ncpu: 7137.4571928395835 ns\nthreads: 1"
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
          "id": "1f9f74503b8f2f63be68be2f6b6ddeb3a18a5750",
          "message": "Fix GitHub Pages performance documentation deployment\n\n- Fix artifact naming conflict between CI and Performance workflows\n- Improve Pages Deploy workflow to properly merge documentation artifacts\n- Update Jekyll configuration to better handle performance files\n- Add performance documentation structure and README\n- Ensure performance data files are properly served through Jekyll\n\nFixes 404 errors on https://wsollers.github.io/utf_strings/performance/",
          "timestamp": "2025-11-02T14:00:07-05:00",
          "tree_id": "d1a2a1fa177ce9ba60e1a64873548d1931831a45",
          "url": "https://github.com/wsollers/utf_strings/commit/1f9f74503b8f2f63be68be2f6b6ddeb3a18a5750"
        },
        "date": 1762110187002,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7044.577643288325,
            "unit": "ns/iter",
            "extra": "iterations: 198446\ncpu: 7043.363489311955 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "1f9f74503b8f2f63be68be2f6b6ddeb3a18a5750",
          "message": "Fix GitHub Pages performance documentation deployment\n\n- Fix artifact naming conflict between CI and Performance workflows\n- Improve Pages Deploy workflow to properly merge documentation artifacts\n- Update Jekyll configuration to better handle performance files\n- Add performance documentation structure and README\n- Ensure performance data files are properly served through Jekyll\n\nFixes 404 errors on https://wsollers.github.io/utf_strings/performance/",
          "timestamp": "2025-11-02T19:00:07Z",
          "url": "https://github.com/wsollers/utf_strings/commit/1f9f74503b8f2f63be68be2f6b6ddeb3a18a5750"
        },
        "date": 1762110467170,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7024.960964629868,
            "unit": "ns/iter",
            "extra": "iterations: 197257\ncpu: 7024.2698611456135 ns\nthreads: 1"
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
          "id": "fc7f7dc62dac1cec2f77376d01e68fda1e913a9e",
          "message": "Fix Pages Deploy artifact handling\n\n- Handle different artifact names from CI vs Performance workflows\n- Improve error handling when artifacts are not found\n- Use repository docs as fallback when no artifacts available",
          "timestamp": "2025-11-02T14:09:49-05:00",
          "tree_id": "e6868fa71ccc6ee2f77ba655b1e8c6cb92b4c707",
          "url": "https://github.com/wsollers/utf_strings/commit/fc7f7dc62dac1cec2f77376d01e68fda1e913a9e"
        },
        "date": 1762110709008,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7040.464007393126,
            "unit": "ns/iter",
            "extra": "iterations: 185038\ncpu: 7039.966947329736 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "fc7f7dc62dac1cec2f77376d01e68fda1e913a9e",
          "message": "Fix Pages Deploy artifact handling\n\n- Handle different artifact names from CI vs Performance workflows\n- Improve error handling when artifacts are not found\n- Use repository docs as fallback when no artifacts available",
          "timestamp": "2025-11-02T19:09:49Z",
          "url": "https://github.com/wsollers/utf_strings/commit/fc7f7dc62dac1cec2f77376d01e68fda1e913a9e"
        },
        "date": 1762111010164,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7236.347330983942,
            "unit": "ns/iter",
            "extra": "iterations: 186698\ncpu: 7235.621217152835 ns\nthreads: 1"
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
          "id": "0fe64d2a61e9b032f40b5f0293db02be8eb9afb2",
          "message": "Fix Performance Baseline docs structure for Pages deployment\n\n- Add debugging to show what files are being uploaded\n- Ensure complete Jekyll site structure is included in artifact\n- Verify presence of _config.yml and index.html for proper Pages deployment",
          "timestamp": "2025-11-02T15:56:05-05:00",
          "tree_id": "4772ec0dedb2974f6b4999ef0e16d18906a77ff2",
          "url": "https://github.com/wsollers/utf_strings/commit/0fe64d2a61e9b032f40b5f0293db02be8eb9afb2"
        },
        "date": 1762117075013,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7024.834979035004,
            "unit": "ns/iter",
            "extra": "iterations: 198902\ncpu: 7023.899196589274 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "0fe64d2a61e9b032f40b5f0293db02be8eb9afb2",
          "message": "Fix Performance Baseline docs structure for Pages deployment\n\n- Add debugging to show what files are being uploaded\n- Ensure complete Jekyll site structure is included in artifact\n- Verify presence of _config.yml and index.html for proper Pages deployment",
          "timestamp": "2025-11-02T20:56:05Z",
          "url": "https://github.com/wsollers/utf_strings/commit/0fe64d2a61e9b032f40b5f0293db02be8eb9afb2"
        },
        "date": 1762117190453,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7049.846659799483,
            "unit": "ns/iter",
            "extra": "iterations: 199015\ncpu: 7049.328829485214 ns\nthreads: 1"
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
          "id": "d6bce6df8174d1b40ed77ad0050e323e63312ad2",
          "message": "Update workflows for proper performance page deployment\n\n- Ensure CI-generated performance pages take precedence over local files\n- Remove conflicting local performance files that override CI data",
          "timestamp": "2025-11-02T16:02:36-05:00",
          "tree_id": "44515bcfe79e7c17aef3e00119944fb650a587eb",
          "url": "https://github.com/wsollers/utf_strings/commit/d6bce6df8174d1b40ed77ad0050e323e63312ad2"
        },
        "date": 1762117461092,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 6918.967853283851,
            "unit": "ns/iter",
            "extra": "iterations: 204531\ncpu: 6917.930025277341 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "d6bce6df8174d1b40ed77ad0050e323e63312ad2",
          "message": "Update workflows for proper performance page deployment\n\n- Ensure CI-generated performance pages take precedence over local files\n- Remove conflicting local performance files that override CI data",
          "timestamp": "2025-11-02T21:02:36Z",
          "url": "https://github.com/wsollers/utf_strings/commit/d6bce6df8174d1b40ed77ad0050e323e63312ad2"
        },
        "date": 1762117464591,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7103.917922250058,
            "unit": "ns/iter",
            "extra": "iterations: 199177\ncpu: 7102.929238817735 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "d6bce6df8174d1b40ed77ad0050e323e63312ad2",
          "message": "Update workflows for proper performance page deployment\n\n- Ensure CI-generated performance pages take precedence over local files\n- Remove conflicting local performance files that override CI data",
          "timestamp": "2025-11-02T21:02:36Z",
          "url": "https://github.com/wsollers/utf_strings/commit/d6bce6df8174d1b40ed77ad0050e323e63312ad2"
        },
        "date": 1762117562777,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7049.614743274718,
            "unit": "ns/iter",
            "extra": "iterations: 195384\ncpu: 7048.837417598165 ns\nthreads: 1"
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
          "id": "56d796d819246942b20d4ee1f1d555cff8e0744a",
          "message": "Fix Performance Baseline workflow step ordering\n\nCRITICAL FIX: Move artifact upload AFTER performance index.html creation\n- Previously uploaded before enhanced HTML was generated\n- Now creates enhanced performance page first, then uploads it\n- Add verification to confirm flamegraph links are present\n- This should fix the static text issue on performance page",
          "timestamp": "2025-11-02T16:10:24-05:00",
          "tree_id": "33b79aa9a54b176f6c5621295caf9c3561ceb738",
          "url": "https://github.com/wsollers/utf_strings/commit/56d796d819246942b20d4ee1f1d555cff8e0744a"
        },
        "date": 1762117986495,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7026.049119513069,
            "unit": "ns/iter",
            "extra": "iterations: 198129\ncpu: 7025.212260698838 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "56d796d819246942b20d4ee1f1d555cff8e0744a",
          "message": "Fix Performance Baseline workflow step ordering\n\nCRITICAL FIX: Move artifact upload AFTER performance index.html creation\n- Previously uploaded before enhanced HTML was generated\n- Now creates enhanced performance page first, then uploads it\n- Add verification to confirm flamegraph links are present\n- This should fix the static text issue on performance page",
          "timestamp": "2025-11-02T21:10:24Z",
          "url": "https://github.com/wsollers/utf_strings/commit/56d796d819246942b20d4ee1f1d555cff8e0744a"
        },
        "date": 1762118036778,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 6873.906272337359,
            "unit": "ns/iter",
            "extra": "iterations: 194457\ncpu: 6872.986027759349 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "56d796d819246942b20d4ee1f1d555cff8e0744a",
          "message": "Fix Performance Baseline workflow step ordering\n\nCRITICAL FIX: Move artifact upload AFTER performance index.html creation\n- Previously uploaded before enhanced HTML was generated\n- Now creates enhanced performance page first, then uploads it\n- Add verification to confirm flamegraph links are present\n- This should fix the static text issue on performance page",
          "timestamp": "2025-11-02T21:10:24Z",
          "url": "https://github.com/wsollers/utf_strings/commit/56d796d819246942b20d4ee1f1d555cff8e0744a"
        },
        "date": 1762118059282,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7029.84187807914,
            "unit": "ns/iter",
            "extra": "iterations: 198309\ncpu: 7029.297807966354 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "56d796d819246942b20d4ee1f1d555cff8e0744a",
          "message": "Fix Performance Baseline workflow step ordering\n\nCRITICAL FIX: Move artifact upload AFTER performance index.html creation\n- Previously uploaded before enhanced HTML was generated\n- Now creates enhanced performance page first, then uploads it\n- Add verification to confirm flamegraph links are present\n- This should fix the static text issue on performance page",
          "timestamp": "2025-11-02T21:10:24Z",
          "url": "https://github.com/wsollers/utf_strings/commit/56d796d819246942b20d4ee1f1d555cff8e0744a"
        },
        "date": 1762118256543,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7057.019655857384,
            "unit": "ns/iter",
            "extra": "iterations: 196837\ncpu: 7056.170013767737 ns\nthreads: 1"
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
          "id": "5fa37a61d194cea916ed11c2d0f352f468638792",
          "message": "🚀 Bump version to 0.1.0 - Performance Monitoring Release\n\nMajor new features in this release:\n- ✅ Complete performance monitoring pipeline with CI integration\n- 🔥 Interactive SVG flamegraph generation with perf profiling\n- 📊 Real-time performance dashboard with Jekyll integration\n- 📈 Historical benchmark tracking with regression detection\n- 🌐 GitHub Pages deployment with automated data updates\n- 🎯 Multiple benchmark output formats (JSON, CSV, console)\n- 💻 System information and build context reporting\n- 🛠️ Robust error handling and fallback mechanisms\n\nReady for release creation! 🎉",
          "timestamp": "2025-11-02T16:16:37-05:00",
          "tree_id": "1fcad53b74e8db31b5d85f257b94c98177306239",
          "url": "https://github.com/wsollers/utf_strings/commit/5fa37a61d194cea916ed11c2d0f352f468638792"
        },
        "date": 1762118477947,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7030.992836567646,
            "unit": "ns/iter",
            "extra": "iterations: 197531\ncpu: 7030.273916499183 ns\nthreads: 1"
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
          "id": "5de336b7b63300ff156a365d9745e8c26d8e2e3b",
          "message": "Fix version number to 0.0.2\n\nCorrect version progression from existing v0.0.1 tag:\n- Previous: v0.0.1 (initial release)\n- Current: v0.0.2 (performance monitoring patch)\n\nMajor features added in v0.0.2:\n- 🔥 Interactive SVG flamegraph generation with perf profiling\n- 📊 Real-time performance dashboard with Jekyll integration\n- 📈 Historical benchmark tracking with regression detection\n- 🌐 GitHub Pages deployment with automated data updates\n- 🎯 Multiple benchmark output formats (JSON, CSV, console)\n- 💻 System information and build context reporting",
          "timestamp": "2025-11-02T16:19:59-05:00",
          "tree_id": "9902d53a06937e621cc9b6e5cacae45edb9c63f5",
          "url": "https://github.com/wsollers/utf_strings/commit/5de336b7b63300ff156a365d9745e8c26d8e2e3b"
        },
        "date": 1762118499885,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7039.460781343108,
            "unit": "ns/iter",
            "extra": "iterations: 198018\ncpu: 7037.616297508309 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "5de336b7b63300ff156a365d9745e8c26d8e2e3b",
          "message": "Fix version number to 0.0.2\n\nCorrect version progression from existing v0.0.1 tag:\n- Previous: v0.0.1 (initial release)\n- Current: v0.0.2 (performance monitoring patch)\n\nMajor features added in v0.0.2:\n- 🔥 Interactive SVG flamegraph generation with perf profiling\n- 📊 Real-time performance dashboard with Jekyll integration\n- 📈 Historical benchmark tracking with regression detection\n- 🌐 GitHub Pages deployment with automated data updates\n- 🎯 Multiple benchmark output formats (JSON, CSV, console)\n- 💻 System information and build context reporting",
          "timestamp": "2025-11-02T21:19:59Z",
          "url": "https://github.com/wsollers/utf_strings/commit/5de336b7b63300ff156a365d9745e8c26d8e2e3b"
        },
        "date": 1762118693601,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7019.88097988208,
            "unit": "ns/iter",
            "extra": "iterations: 199126\ncpu: 7019.149548527063 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "id": "5de336b7b63300ff156a365d9745e8c26d8e2e3b",
          "message": "Fix version number to 0.0.2\n\nCorrect version progression from existing v0.0.1 tag:\n- Previous: v0.0.1 (initial release)\n- Current: v0.0.2 (performance monitoring patch)\n\nMajor features added in v0.0.2:\n- 🔥 Interactive SVG flamegraph generation with perf profiling\n- 📊 Real-time performance dashboard with Jekyll integration\n- 📈 Historical benchmark tracking with regression detection\n- 🌐 GitHub Pages deployment with automated data updates\n- 🎯 Multiple benchmark output formats (JSON, CSV, console)\n- 💻 System information and build context reporting",
          "timestamp": "2025-11-02T21:19:59Z",
          "url": "https://github.com/wsollers/utf_strings/commit/5de336b7b63300ff156a365d9745e8c26d8e2e3b"
        },
        "date": 1762118866473,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_Length_Mixed",
            "value": 7066.255972370754,
            "unit": "ns/iter",
            "extra": "iterations: 199209\ncpu: 7065.885557379437 ns\nthreads: 1"
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "febe201b96fd33847c801b47d5947bc69d6ef984",
          "message": "Merge pull request #3 from wsollers/refactor/utf-implementation\n\nRefactor UTF implementation to modern C++23 CodePoint API with compre…",
          "timestamp": "2025-11-02T19:21:58-05:00",
          "tree_id": "afa3f07eb2b2e39446aedf65c344a5bf68e4720c",
          "url": "https://github.com/wsollers/utf_strings/commit/febe201b96fd33847c801b47d5947bc69d6ef984"
        },
        "date": 1762129601807,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_UTF8_CodePoint_Creation",
            "value": 7.743719973564989,
            "unit": "ns/iter",
            "extra": "iterations: 179770342\ncpu: 7.743254062452639 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_CodePoint_Creation",
            "value": 1.3095776427481025,
            "unit": "ns/iter",
            "extra": "iterations: 1074426325\ncpu: 1.3093962696790775 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF32LE_CodePoint_Creation",
            "value": 0.6226702838559575,
            "unit": "ns/iter",
            "extra": "iterations: 2249461115\ncpu: 0.622501092667254 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Scalar_Conversion",
            "value": 2.4243739789042285,
            "unit": "ns/iter",
            "extra": "iterations: 578150964\ncpu: 2.424083701778624 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Validation",
            "value": 4.668523326828316,
            "unit": "ns/iter",
            "extra": "iterations: 299971428\ncpu: 4.668041070898259 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_to_UTF16BE_Conversion",
            "value": 4.674494221746377,
            "unit": "ns/iter",
            "extra": "iterations: 299966930\ncpu: 4.674014205499254 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_to_UTF32LE_Conversion",
            "value": 2.5394615438693573,
            "unit": "ns/iter",
            "extra": "iterations: 558265981\ncpu: 2.5392391624163824 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Units_Access",
            "value": 2.176938697373954,
            "unit": "ns/iter",
            "extra": "iterations: 643376484\ncpu: 2.1767289041916547 ns\nthreads: 1"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "name": "BoondockTaints",
            "username": "wsollers",
            "email": "wsollers@gmail.com"
          },
          "committer": {
            "name": "GitHub",
            "username": "web-flow",
            "email": "noreply@github.com"
          },
          "id": "febe201b96fd33847c801b47d5947bc69d6ef984",
          "message": "Merge pull request #3 from wsollers/refactor/utf-implementation\n\nRefactor UTF implementation to modern C++23 CodePoint API with compre…",
          "timestamp": "2025-11-03T00:21:58Z",
          "url": "https://github.com/wsollers/utf_strings/commit/febe201b96fd33847c801b47d5947bc69d6ef984"
        },
        "date": 1762129924984,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_UTF8_CodePoint_Creation",
            "value": 7.740581221922665,
            "unit": "ns/iter",
            "extra": "iterations: 179529498\ncpu: 7.739744507055883 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_CodePoint_Creation",
            "value": 1.310565756037269,
            "unit": "ns/iter",
            "extra": "iterations: 1066716235\ncpu: 1.3103794178214603 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF32LE_CodePoint_Creation",
            "value": 0.6239718458446083,
            "unit": "ns/iter",
            "extra": "iterations: 2250147984\ncpu: 0.6239201612439373 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Scalar_Conversion",
            "value": 2.467804559735419,
            "unit": "ns/iter",
            "extra": "iterations: 538574977\ncpu: 2.4675938778343958 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Validation",
            "value": 4.678454142020315,
            "unit": "ns/iter",
            "extra": "iterations: 299759128\ncpu: 4.678008504214762 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_to_UTF16BE_Conversion",
            "value": 4.677581791950199,
            "unit": "ns/iter",
            "extra": "iterations: 299694743\ncpu: 4.677110966207368 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_to_UTF32LE_Conversion",
            "value": 2.614356415478074,
            "unit": "ns/iter",
            "extra": "iterations: 535789105\ncpu: 2.614099948896869 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Units_Access",
            "value": 2.175082277070622,
            "unit": "ns/iter",
            "extra": "iterations: 643877141\ncpu: 2.17488455301444 ns\nthreads: 1"
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
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "cf3dd03945c46025b8680e1a05d1c5eeed8d39fe",
          "message": "Merge pull request #4 from wsollers/feature/utf-strings\n\nAdd Comprehensive UTF Strings Fuzzing and Benchmarking Infrastructure",
          "timestamp": "2025-11-02T23:16:29-05:00",
          "tree_id": "e97e6db11987e777645042e01a9702d93fded792",
          "url": "https://github.com/wsollers/utf_strings/commit/cf3dd03945c46025b8680e1a05d1c5eeed8d39fe"
        },
        "date": 1762144118560,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "BM_UTF8_CodePoint_Creation",
            "value": 6.334287023000245,
            "unit": "ns/iter",
            "extra": "iterations: 220593771\ncpu: 6.333343569343126 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_CodePoint_Creation",
            "value": 1.858275452214576,
            "unit": "ns/iter",
            "extra": "iterations: 729907568\ncpu: 1.8579550897326769 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF32LE_CodePoint_Creation",
            "value": 0.6220269881828455,
            "unit": "ns/iter",
            "extra": "iterations: 2250672983\ncpu: 0.6219323142779293 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Scalar_Conversion",
            "value": 2.4026875387894036,
            "unit": "ns/iter",
            "extra": "iterations: 582957093\ncpu: 2.4022471255873366 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Validation",
            "value": 3.3135093802283464,
            "unit": "ns/iter",
            "extra": "iterations: 422453044\ncpu: 3.3130234090584527 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_to_UTF16BE_Conversion",
            "value": 4.525704198240331,
            "unit": "ns/iter",
            "extra": "iterations: 308075997\ncpu: 4.525135974809485 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_to_UTF32LE_Conversion",
            "value": 2.674794310638833,
            "unit": "ns/iter",
            "extra": "iterations: 524464656\ncpu: 2.6744924752374537 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_Units_Access",
            "value": 2.6117481063732106,
            "unit": "ns/iter",
            "extra": "iterations: 532627370\ncpu: 2.611489437728294 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_FromBytes",
            "value": 137.3972423971855,
            "unit": "ns/iter",
            "extra": "iterations: 10239038\ncpu: 137.38480138466116 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_String_FromBytes",
            "value": 56.475405441753374,
            "unit": "ns/iter",
            "extra": "iterations: 25140541\ncpu: 56.46899476029576 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF32LE_String_FromBytes",
            "value": 24.688661831182785,
            "unit": "ns/iter",
            "extra": "iterations: 57589961\ncpu: 24.687169453023238 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_FromCString",
            "value": 17.937792033486748,
            "unit": "ns/iter",
            "extra": "iterations: 77965802\ncpu: 17.93655059175813 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_CopyConstruction",
            "value": 6.701796491424881,
            "unit": "ns/iter",
            "extra": "iterations: 215979823\ncpu: 6.70132562336622 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_Assignment",
            "value": 18.37607510987636,
            "unit": "ns/iter",
            "extra": "iterations: 76390913\ncpu: 18.375200477051482 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_to_UTF16BE_String_Conversion",
            "value": 192.3244667845576,
            "unit": "ns/iter",
            "extra": "iterations: 7275999\ncpu: 192.3164046614083 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF16BE_to_UTF32LE_String_Conversion",
            "value": 108.39888923265814,
            "unit": "ns/iter",
            "extra": "iterations: 12918817\ncpu: 108.389917977784 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_RoundTrip_Conversion",
            "value": 391.8451013949962,
            "unit": "ns/iter",
            "extra": "iterations: 3578825\ncpu: 391.818638240205 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_Concatenation",
            "value": 21.633759051675835,
            "unit": "ns/iter",
            "extra": "iterations: 64919057\ncpu: 21.632565057129497 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_Append_Operator",
            "value": 18.94017911715153,
            "unit": "ns/iter",
            "extra": "iterations: 73560683\ncpu: 18.93898483786504 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_CodePoint_Counting",
            "value": 39.122958247484846,
            "unit": "ns/iter",
            "extra": "iterations: 39257635\ncpu: 39.12057631591929 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_Iteration",
            "value": 92.8569688729083,
            "unit": "ns/iter",
            "extra": "iterations: 15104817\ncpu: 92.85199284440208 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_String_Validation",
            "value": 129.31141037042846,
            "unit": "ns/iter",
            "extra": "iterations: 10939899\ncpu: 129.30378781376356 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_StringView_Creation",
            "value": 2.89440231861894,
            "unit": "ns/iter",
            "extra": "iterations: 482825355\ncpu: 2.894293751412453 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_StringView_CodePoint_Counting",
            "value": 39.04120128631543,
            "unit": "ns/iter",
            "extra": "iterations: 37453952\ncpu: 39.03911360275144 ns\nthreads: 1"
          },
          {
            "name": "BM_UTF8_StringView_Iteration",
            "value": 94.03634972196208,
            "unit": "ns/iter",
            "extra": "iterations: 14813951\ncpu: 94.03083674301334 ns\nthreads: 1"
          }
        ]
      }
    ]
  }
}