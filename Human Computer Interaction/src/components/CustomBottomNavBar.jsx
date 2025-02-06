import React from "react";
import { useNavigate, useLocation } from "react-router-dom";
import FitnessCenterIcon from "@mui/icons-material/FitnessCenter";
import HomeIcon from "@mui/icons-material/Home";
import SettingsIcon from "@mui/icons-material/Settings";
import BottomNavigation from "@mui/material/BottomNavigation";
import BottomNavigationAction from "@mui/material/BottomNavigationAction";

export default function LabelBottomNavigation() {
    const navigate = useNavigate();
    const location = useLocation(); // Hook to get the current location

    const getValueFromPath = (path) => {
        switch (path) {
            case "/":
                return "home";
            case "/routinen":
                return "workout";
            case "/workout":
                return "workout";
            case "/settings":
                return "settings";
            default:
                return "home"; // Fallback to home
        }
    };

    const [value, setValue] = React.useState(getValueFromPath(location.pathname));

    // Synchronize value with the current path when location changes
    React.useEffect(() => {
        setValue(getValueFromPath(location.pathname));
    }, [location.pathname]);

    const handleChange = (event, newValue) => {
        setValue(newValue); // Update the value state
        if (newValue === "workout") {
            navigate("/routinen");
        } else if (newValue === "home") {
            navigate("/");
        } else if (newValue === "settings") {
            navigate("/settings");
        }
    };

    return (
        <BottomNavigation
            sx={{ zIndex: 1300, position: "fixed", bottom: 0, width: 1.0 }}
            value={value}
            onChange={handleChange}
        >
            <BottomNavigationAction label="Home" value="home" icon={<HomeIcon />} />
            <BottomNavigationAction label="Workout" value="workout" icon={<FitnessCenterIcon />} />
            <BottomNavigationAction label="Settings" value="settings" icon={<SettingsIcon />} />
        </BottomNavigation>
    );
}