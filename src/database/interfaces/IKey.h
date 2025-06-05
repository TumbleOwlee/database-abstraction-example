#pragma once

namespace persistence {

namespace interface {

/**!
 * \brief Interface representing the key handle to an object stored in the database
 */
class IKey {
public:
    virtual ~IKey() {}
};

} // namespace interface

} // namespace persistence
