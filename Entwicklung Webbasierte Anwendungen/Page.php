<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

abstract class Page
{
    // --- ATTRIBUTES ---
    protected MySQLi $_database;

    // --- OPERATIONS ---
    protected function __construct()
    {
        error_reporting(E_ALL);

        $host = "localhost";
        /********************************************/
        // This code switches from the the local installation (XAMPP) to the docker installation
        if (gethostbyname('mariadb') != "mariadb") { // mariadb is known?
            $host = "mariadb";
        }
        /********************************************/

        $this->_database = new MySQLi($host, "public", "public", "pizzaservice");

        if (mysqli_connect_errno()) {
            throw new Exception("Connect failed: " . mysqli_connect_error());
        }

        // set charset to UTF8!!
        if (!$this->_database->set_charset("utf8")) {
            throw new Exception($this->_database->error);
        }
    }

    public function __destruct()
    {
        // to do: close database
        $this->_database->close();
        // session_destroy();
    }

    protected function generatePageHeader(string $title = "", string $jsFile = "", bool $autoreload = false):void
    {
        $title = htmlspecialchars($title);
        header("Content-type: text/html; charset=UTF-8");
        echo <<< HTML
            <!DOCTYPE html>
            <html lang="de">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
        HTML;
        if($autoreload)
            echo '<meta http-equiv="refresh" content="10">';
        if($jsFile != "")
            echo '<script src=' . $jsFile . ' defer></script>';
        echo <<< HTML
                <link rel="stylesheet" href="../Assets/Style/style.css">
                <title>$title</title>
            </head>
            <body>
                <header class="section">
                    <h1>PIZZA-SERVICE</h1>
                    <nav>
                        <ul>
        HTML;
        if($title == "Bestellung")
        {
            echo '<li>'.'<a class="active" href="bestellung.php">'.'HOME'.'</a>'.'</li>';
            echo '<li>'.'<a href="kunde.php">'.'KUNDE'.'</a>'.'</li>';
            echo '<li>'.'<a href="baecker.php">'.'BÄKER'.'</a>'.'</li>';
            echo '<li>'.'<a href="fahrer.php">'.'FAHRER'.'</a>'.'</li>';
        }
        elseif($title == "Kunde")
        {
            echo '<li>'.'<a href="bestellung.php">'.'HOME'.'</a>'.'</li>';
            echo '<li>'.'<a class="active" href="kunde.php">'.'KUNDE'.'</a>'.'</li>';
            echo '<li>'.'<a href="baecker.php">'.'BÄKER'.'</a>'.'</li>';
            echo '<li>'.'<a href="fahrer.php">'.'FAHRER'.'</a>'.'</li>';
        }
        elseif($title == "Baecker")
        {
            echo '<li>'.'<a href="bestellung.php">'.'HOME'.'</a>'.'</li>';
            echo '<li>'.'<a href="kunde.php">'.'KUNDE'.'</a>'.'</li>';
            echo '<li>'.'<a class="active" href="baecker.php">'.'BÄKER'.'</a>'.'</li>';
            echo '<li>'.'<a href="fahrer.php">'.'FAHRER'.'</a>'.'</li>';
        }
        elseif($title == "Fahrer")
        {
            echo '<li>'.'<a href="bestellung.php">'.'HOME'.'</a>'.'</li>';
            echo '<li>'.'<a href="kunde.php">'.'KUNDE'.'</a>'.'</li>';
            echo '<li>'.'<a href="baecker.php">'.'BÄKER'.'</a>'.'</li>';
            echo '<li>'.'<a class="active" href="fahrer.php">'.'FAHRER'.'</a>'.'</li>';
        }
        echo <<< HTML
            </ul>
            </nav>
            </header>
        HTML;

        // to do: handle all parameters
        // to do: output common beginning of HTML code
    }

    protected function generatePageFooter():void
    {
        echo <<< HTML
            <footer class="section">
            <p>&copy; Dester Ngouateu</p>
            <p>Fachbereich Informatik</p>
            </footer>
        </body>
        </html>
        HTML;
        // to do: output common end of HTML code
    }

    protected function processReceivedData():void
    {

    }
} // end of class

// Zend standard does not like closing php-tag!
// PHP doesn't require the closing tag (it is assumed when the file ends).
// Not specifying the closing ? >  helps to prevent accidents
// like additional whitespace which will cause session
// initialization to fail ("headers already sent").
//? >