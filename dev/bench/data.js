window.BENCHMARK_DATA = {
  "lastUpdate": 1762069884251,
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
      }
    ]
  }
}