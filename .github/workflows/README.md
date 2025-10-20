# GitHub Actions Workflows

This directory contains GitHub Actions workflows for continuous integration and testing of wkhtmltopdf.

## Available Workflows

### build-and-test.yml
**Purpose**: Build and test wkhtmltopdf on multiple platforms using system Qt packages.

**Triggers**: 
- Push to `master` or `main` branches
- Pull requests targeting `master` or `main` branches

**Jobs**:
1. **build-linux-qt5** (Ubuntu 20.04)
   - Builds wkhtmltopdf and wkhtmltoimage with Qt5
   - Runs comprehensive smoke tests:
     - Verifies binary creation
     - Tests basic PDF generation from HTML
     - Tests image generation from HTML
     - Tests with complex HTML formatting
     - Builds and validates C API examples
   - Uploads build artifacts and test outputs

2. **build-linux-qt5-ubuntu-22** (Ubuntu 22.04)
   - Builds on newer Ubuntu for compatibility testing
   - Runs basic smoke tests

3. **static-analysis**
   - Performs code quality checks
   - Verifies build files are present
   - Checks file permissions

**Artifacts**:
- `wkhtmltopdf-linux-qt5-binaries`: Built binaries (wkhtmltopdf, wkhtmltoimage, libwkhtmltox.so)
- `test-outputs-linux-qt5`: Generated test PDFs and images

### official.yml
**Purpose**: Build official packages using the patched Qt from wkhtmltopdf/packaging repository.

**Platforms**: Linux (Docker), macOS, Windows

**Details**: See inline comments in the workflow file.

### unpatched.yml
**Purpose**: Build using unpatched system Qt packages (Qt4 and Qt5).

**Platforms**: Ubuntu 18.04 (Qt4, legacy), Ubuntu 20.04 (Qt5)

**Details**: Simpler builds for testing compatibility with unpatched Qt. Note: Ubuntu 18.04 is used for Qt4 legacy support only.

## Workflow Design Philosophy

The `build-and-test.yml` workflow was designed based on the following principles borrowed from the [wkhtmltopdf/packaging](https://github.com/wkhtmltopdf/packaging) repository:

1. **Multi-platform testing**: Test on different Ubuntu versions to ensure compatibility
2. **Comprehensive smoke tests**: Don't just build - verify the binaries actually work
3. **Artifact preservation**: Save build outputs for debugging and distribution
4. **Fast feedback**: Use system packages for quick CI feedback (official builds use packaging repo)

## Testing Approach

Tests include:
- **Build verification**: Ensure all expected binaries are created
- **Version check**: Verify binaries can report their version
- **Basic functionality**: Generate PDFs and images from simple HTML
- **Complex HTML**: Test with styled HTML including CSS
- **C API examples**: Verify the library API works correctly

## Adding New Tests

To add new tests to `build-and-test.yml`:

1. Add a new step under the appropriate job
2. Use `export LD_LIBRARY_PATH=$PWD/bin:$LD_LIBRARY_PATH` before running binaries
3. Include verification steps (e.g., `test -f output.pdf`)
4. Consider uploading outputs as artifacts for debugging

## Local Testing

To run similar tests locally:

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install qtbase5-dev libqt5webkit5-dev libqt5xmlpatterns5-dev libqt5svg5-dev

# Build
qmake CONFIG+=silent
make -j$(nproc)

# Test
export LD_LIBRARY_PATH=$PWD/bin:$LD_LIBRARY_PATH
echo '<html><body><h1>Test</h1></body></html>' > test.html
./bin/wkhtmltopdf test.html test.pdf
```

## Maintenance Notes

- The `build-and-test.yml` workflow uses system Qt packages for speed
- For official releases, use the `official.yml` workflow which uses the packaging repository
- Keep workflow Ubuntu versions aligned with active LTS releases (except where legacy support is needed, like Qt4 in `unpatched.yml`)
- Update action versions (e.g., `actions/checkout@v2`) periodically
