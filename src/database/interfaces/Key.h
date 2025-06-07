#pragma once

namespace persistence {

namespace interface {

/**!
 * \brief Interface representing the key handle to an object stored in the database
 */
class Key {
public:
    virtual ~Key() {}
};

} // namespace interface

} // namespace persistence
