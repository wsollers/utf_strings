window.BENCHMARK_DATA = {
  "lastUpdate": 1762062592260,
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
      }
    ]
  }
}