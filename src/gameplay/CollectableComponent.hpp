#pragma once

enum class CollectableType {
    Default,  // Existing collectible
    Feather,
    Potion,
};

struct CollectableComponent {
    CollectableType type = CollectableType::Default;
};