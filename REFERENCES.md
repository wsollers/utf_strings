# References

This document contains references to the official specifications and standards that define UTF-8, UTF-16, UTF-32, and related Unicode encoding standards used in the UTF Strings library.

## Unicode and UTF Encoding Standards

| Standard | Name | URL | Description |
|----------|------|-----|-------------|
| **RFC 3629** | UTF-8, a transformation format of ISO 10646 | https://tools.ietf.org/rfc/rfc3629.txt | Official specification for UTF-8 encoding |
| **RFC 2781** | UTF-16, an encoding of ISO 10646 | https://tools.ietf.org/rfc/rfc2781.txt | Official specification for UTF-16 encoding |
| **Unicode Standard** | The Unicode Standard (Latest Version) | https://www.unicode.org/versions/latest/ | Complete Unicode specification including UTF-32 |
| **ISO/IEC 10646** | Information technology — Universal Coded Character Set (UCS) | https://www.iso.org/standard/76835.html | International standard for Unicode |

## Core UTF Encoding Specifications

### UTF-8

| Document | Name | URL | Notes |
|----------|------|-----|-------|
| **RFC 3629** | UTF-8, a transformation format of ISO 10646 | https://datatracker.ietf.org/doc/html/rfc3629 | Current UTF-8 standard (obsoletes RFC 2279) |
| **RFC 2279** | UTF-8, a transformation format of ISO 10646 | https://datatracker.ietf.org/doc/html/rfc2279 | Original UTF-8 specification (obsoleted by RFC 3629) |

### UTF-16

| Document | Name | URL | Notes |
|----------|------|-----|-------|
| **RFC 2781** | UTF-16, an encoding of ISO 10646 | https://datatracker.ietf.org/doc/html/rfc2781 | Official UTF-16 specification |
| **Unicode Technical Report #17** | Unicode Character Encoding Model | https://www.unicode.org/reports/tr17/ | Character encoding architecture |

### UTF-32

| Document | Name | URL | Notes |
|----------|------|-----|-------|
| **Unicode Standard Chapter 3** | Conformance | https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf | UTF-32 definition (Section 3.9) |
| **ISO/IEC 10646** | Universal Coded Character Set | https://www.iso.org/standard/76835.html | International standard including UTF-32 |

## Unicode Technical Reports and Standards

| Document | Name | URL | Description |
|----------|------|-----|-------------|
| **UTR #17** | Unicode Character Encoding Model | https://www.unicode.org/reports/tr17/ | Conceptual model for character encodings |
| **UTR #36** | Unicode Security Considerations | https://www.unicode.org/reports/tr36/ | Security implications of Unicode |
| **UTR #15** | Unicode Normalization Forms | https://www.unicode.org/reports/tr15/ | Canonical and compatibility equivalence |
| **UTR #9** | Unicode Bidirectional Algorithm | https://www.unicode.org/reports/tr9/ | Bidirectional text handling |
| **UTR #29** | Unicode Text Segmentation | https://www.unicode.org/reports/tr29/ | Grapheme, word, and sentence boundaries |

## Byte Order Mark (BOM) Specifications

| Document | Name | URL | Description |
|----------|------|-----|-------------|
| **Unicode FAQ BOM** | Byte Order Mark (BOM) FAQ | https://www.unicode.org/faq/utf_bom.html | Official BOM documentation |
| **Unicode Standard Appendix** | Byte Order Mark | https://www.unicode.org/versions/Unicode15.0.0/appA.pdf | BOM specifications in Unicode Standard |

## Security and Best Practices

| Document | Name | URL | Description |
|----------|------|-----|-------------|
| **RFC 6365** | Terminology Used in Internationalization in the IETF | https://datatracker.ietf.org/doc/html/rfc6365 | I18N terminology and concepts |
| **UTR #36** | Unicode Security Considerations | https://www.unicode.org/reports/tr36/ | Security implications and best practices |
| **UTR #39** | Unicode Security Mechanisms | https://www.unicode.org/reports/tr39/ | Security mechanisms for Unicode text |

## Implementation Guidelines

| Document | Name | URL | Description |
|----------|------|-----|-------------|
| **Unicode Standard Annex #31** | Unicode Identifier and Pattern Syntax | https://www.unicode.org/reports/tr31/ | Guidelines for identifiers |
| **Unicode Consortium Guidelines** | Unicode Encoding Guidelines | https://www.unicode.org/standard/principles.html | General encoding principles |
| **W3C Character Model** | Character Model for the World Wide Web 1.0 | https://www.w3.org/TR/charmod/ | Web-specific character handling |

## Historical and Obsolete Standards (For Reference)

| Document | Name | URL | Status |
|----------|------|-----|--------|
| **RFC 2279** | UTF-8, a transformation format of ISO 10646 | https://datatracker.ietf.org/doc/html/rfc2279 | Obsoleted by RFC 3629 |
| **RFC 1641** | Using Unicode with MIME | https://datatracker.ietf.org/doc/html/rfc1641 | Historical reference |

## Related Internet Standards

| Document | Name | URL | Description |
|----------|------|-----|-------------|
| **RFC 5198** | Unicode Format for Network Interchange | https://datatracker.ietf.org/doc/html/rfc5198 | Network interchange format |
| **RFC 4013** | SASLprep: Stringprep Profile for User Names and Passwords | https://datatracker.ietf.org/doc/html/rfc4013 | Username/password preparation |
| **RFC 3454** | Preparation of Internationalized Strings | https://datatracker.ietf.org/doc/html/rfc3454 | String preparation framework |

## Testing and Conformance

| Document | Name | URL | Description |
|----------|------|-----|-------------|
| **Unicode Test Data** | Unicode Character Database | https://www.unicode.org/ucd/ | Official Unicode test data |
| **Unicode Conformance Tests** | Unicode Test Suite | https://www.unicode.org/reports/tr18/#Conformance_Tests | Conformance testing guidelines |
| **UTF-8 Test Vectors** | UTF-8 decoder capability and stress test | https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt | Comprehensive UTF-8 test cases |

## Implementation Notes

### Key Points for UTF Strings Library

1. **UTF-8**: Defined in RFC 3629, variable-length encoding (1-4 bytes)
2. **UTF-16**: Defined in RFC 2781, variable-length encoding (2-4 bytes) with surrogates
3. **UTF-32**: Defined in Unicode Standard, fixed-length encoding (4 bytes)
4. **Endianness**: UTF-16 and UTF-32 require byte order considerations
5. **Security**: UTR #36 provides crucial security considerations for implementation

### Validation Requirements

- **Overlong sequences**: Must be rejected (UTF-8)
- **Surrogate pairs**: Must be properly handled (UTF-16)
- **Code point range**: Must validate against Unicode scalar values
- **Byte order marks**: Must be handled according to specification

### Conformance Levels

The UTF Strings library aims for **full conformance** with:
- RFC 3629 (UTF-8)
- RFC 2781 (UTF-16) 
- Unicode Standard (UTF-32)
- Security best practices from UTR #36

## Additional Resources

| Resource | Name | URL | Description |
|----------|------|-----|-------------|
| **Unicode Consortium** | Official Unicode Website | https://www.unicode.org/ | Primary source for Unicode information |
| **IANA Character Sets** | Character Sets Registry | https://www.iana.org/assignments/character-sets/character-sets.xhtml | Official character set registry |
| **W3C Internationalization** | I18N Activity | https://www.w3.org/International/ | Web internationalization resources |

---

**Last Updated:** November 2025  
**UTF Strings Library Version:** 0.1.0  
**Maintained by:** UTF Strings Development Team