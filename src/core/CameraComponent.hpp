// Simple camera component that just follows the player.
// TODO: implement better camera that uses space interpolation of some sort
struct CameraComponent {
    float y_offset = 0.0f;  // Tracks the camera's vertical position

    // Update the camera to match the player's y-position
    void follow_player(float player_y) {
        y_offset = player_y;
    }
};
