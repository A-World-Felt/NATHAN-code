#ifndef DEMO_EVENTS_DEMO_EVENT_TYPES_H_
#define DEMO_EVENTS_DEMO_EVENT_TYPES_H_

#include <string_view>

namespace demo::events {

// Demo game-specific events
inline constexpr std::string_view collision = "collision";
inline constexpr std::string_view score = "score";
inline constexpr std::string_view position_update = "position_update";
inline constexpr std::string_view welcome = "welcome";
inline constexpr std::string_view player_jump = "player_jump";

} // namespace demo::events

#endif // DEMO_EVENTS_DEMO_EVENT_TYPES_H_
