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
        void
    ) :
        sv_major(0),
        sv_minor(0),
        sv_patch(0),
        sv_prerelease(0)
    {}

    SemVer (
        const size_t sv_major_,
        const size_t sv_minor_,
        const size_t sv_patch_,
        const size_t sv_prerelease_
    ) :
        sv_major(sv_major_),
        sv_minor(sv_minor_),
        sv_patch(sv_patch_),
        sv_prerelease(sv_prerelease_)
    {}

    size_t sv_major : 8;
    size_t sv_minor : 8;
    size_t sv_patch : 8;
    size_t sv_prerelease : 8;
};

}  // namespace remote_wiring

#endif  // SEMANTIC_VERSION_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
