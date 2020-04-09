#pragma once

namespace TNG {
	entt::entity GetFirstLayerRelatedTo(entt::registry& R, entt::entity e); // (e.g. if you click on the handle of a polygon, this will return the associated polygon layer)
}
