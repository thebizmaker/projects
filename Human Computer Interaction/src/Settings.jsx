import HelpIcon from "@mui/icons-material/Help";
import NotificationsIcon from "@mui/icons-material/Notifications";
import PersonIcon from "@mui/icons-material/Person";
import StarIcon from "@mui/icons-material/Star";
import { Box, Divider, List, ListItem, ListItemButton, ListItemIcon, ListItemText, Typography } from "@mui/material";
import React from "react";
import { Link } from "react-router-dom";

const Settings = (props) => {
    return (
      <Box sx={{ mx: 1, mt: 10, mb: 10 }}>

      {/* List of Settings */}
        <Box sx={{ flex: 1, padding: "8px" }}>
        <List>
          {/* Körperdaten */}
            <ListItem disablePadding>
            <ListItemButton component={Link} to="/koerperdaten">
                <ListItemIcon>
                <PersonIcon sx={{ color: "#fff" }} />
                </ListItemIcon>
                <ListItemText primary="Körperdaten" />
            </ListItemButton>
            </ListItem>

          {/* Notifications */}
            <ListItem disablePadding>
            <ListItemButton>
                <ListItemIcon>
                <NotificationsIcon sx={{ color: "#fff" }} />
                </ListItemIcon>
                <ListItemText primary="Notifications" />
            </ListItemButton>
            </ListItem>
        </List>

        {/* Divider */}
        {/* <Divider sx={{ backgroundColor: "#333" }} /> */}

        {/* More Section */}
        <Typography sx={{ margin: "16px 16px 8px", fontSize: "18px", color: "#fff", fontWeight: "bold" }}>More</Typography>
        <List>
          {/* Rate & Review */}
            <ListItem disablePadding>
            <ListItemButton>
                <ListItemIcon>
                <StarIcon sx={{ color: "#fff" }} />
                </ListItemIcon>
                <ListItemText primary="Rate & Review" />
            </ListItemButton>
            </ListItem>

          {/* Help */}
            <ListItem disablePadding>
            <ListItemButton>
                <ListItemIcon>
                <HelpIcon sx={{ color: "#fff" }} />
                </ListItemIcon>
                <ListItemText primary="Help" />
            </ListItemButton>
            </ListItem>
        </List>
        </Box>

    </Box>
    );
};

export default Settings;
