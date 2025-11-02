window.BENCHMARK_DATA = {
  "lastUpdate": 1762063760336,
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
      }
    ]
  }
}