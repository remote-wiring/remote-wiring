/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef SEMANTIC_VERSION_H
#define SEMANTIC_VERSION_H

namespace remote_wiring {

/*!
 * \brief Semantic Versioning Data
 *
 * \sa <a href="https://www.semver.org">Semantic Versioning</a>
 */
struct SemVer {
    SemVer (
        const size_t sv_major_,
        const size_t sv_minor_,
        const size_t sv_patch_
    ) :
        sv_major(sv_major_),
        sv_minor(sv_minor_),
        sv_patch(sv_patch_)
    {}

    bool operator== (
        const SemVer & other_
    ) const {
        return (
            (sv_major == other_.sv_major)
         && (sv_minor == other_.sv_minor)
         && (sv_patch == other_.sv_patch)
        );
    }

    bool operator< (
        const SemVer & other_
    ) const {
        return (
            (sv_major < other_.sv_major)
         || (
                (sv_major == other_.sv_major)
             && (sv_minor < other_.sv_minor)
            )
         || (
                (sv_major == other_.sv_major)
             && (sv_minor == other_.sv_minor)
             && (sv_patch < other_.sv_patch)
            )
        );
    }

    size_t sv_major;
    size_t sv_minor;
    size_t sv_patch;
};

}  // namespace remote_wiring

#endif  // SEMANTIC_VERSION_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
