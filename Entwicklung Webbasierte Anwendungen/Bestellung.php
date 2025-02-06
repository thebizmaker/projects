<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

// to do: change name 'PageTemplate' throughout this file
require_once './Page.php';

class Bestellung extends Page
{
    protected function __construct()
    {
        parent::__construct();
        // to do: instantiate members representing substructures/blocks
    }

    public function __destruct()
    {
        parent::__destruct();
    }

    protected function getViewData():array
    {
        // to do: fetch data for this view from the database
		// to do: return array containing data
        $sqlQuery = "SELECT * FROM `article`";
        $recordset = $this->_database->query($sqlQuery);
        if (!$recordset) {
            throw new Exception("Abfrage fehlgeschlagen: " . $this->_database->error);
        }
        $result = array();
        $record = $recordset->fetch_assoc();
        while ($record){
            $result[] = $record;
            $record = $recordset->fetch_assoc();
        }
        $recordset->free();
        return $result;
    }

    protected function generateView():void
    {
        $data = $this->getViewData(); //NOSONAR ignore unused $data
        $this->generatePageHeader('Bestellung', '../Assets/JS/main.js'); //to do: set optional parameters
        // to do: output view of this page
        $totalPrice = 0;
        echo '<section class="section">';
        echo '<h2>' . 'Speisekarte' . '</h2>' . '<hr>';
        echo '<div class="wraper">';
        foreach($data as $index => $item){
            echo '<div class="item">';
            echo '<div class="image">';
            echo '<img data-price="'.$item['price'].'" src="' . htmlspecialchars($item['picture']) . '" alt="' . htmlspecialchars($item['name']) .'">';
            echo '</div>' . '<p>';
            echo htmlspecialchars($item['name']);
            echo '</p>' . '<p>';
            echo htmlspecialchars($item['price']) . '€';
            echo '</p>' . '</div>';
            $totalPrice += (float)$item['price'];
        }
        echo '</div>' . '</section>';
        echo <<< HTML
            <section class="section">
                <h2>Warenkorb</h2>
                <hr>
                <form action="Bestellung.php" method="post">
                    <div id="wraper" class="wraper">
                        <input id="adresse" type="text" name="adresse" placeholder="Adresse eingeben" tabindex="2" value="" required>
                        <select name="warenkorb[]" id="warenkorb" size="6" multiple tabindex="1" title="warenkorb" required></select>
                        <button id="button1" class="button1" type="button" tabindex="3" accesskey="0">
                            <span>Alles Löschen</span></button>
                        <button id="button2" class="button2" type="button" tabindex="4" accesskey="1"><span>Auswahl Löschen</span></button>
                        <button id="button3" class="button3" type="submit" accesskey="2" tabindex="5" disabled><span>Bestellen</span></button>
                        <div class="div-gpreis">
                            <p id="gesamtpreis">Gesamtpreis: 0 €</p>
                        </div>
                    </div>
                </form>
            </section>
        HTML;
        $this->generatePageFooter();
    }

    protected function processReceivedData():void
    {
        parent::processReceivedData();
        // to do: call processReceivedData() for all members
        if (isset($_POST['adresse']) && isset($_POST['warenkorb']))
        {
            $adresse = $this->_database->real_escape_string($_POST['adresse']);
            $sqlQuery = "INSERT INTO `ordering` (`address`) VALUES ('" . $adresse . "')";
            $this->_database->query($sqlQuery);
            $ordering_id = (int)$this->_database->insert_id;

            $articles = $this->getViewData();

            if (isset($_SESSION['ordered_article']))
                $session_id = $_SESSION['ordered_article'];
            else
                $session_id = array();
            foreach ($articles as $index => $item){
                foreach($_POST['warenkorb'] as $pizza){
                    if($item['name'] == $pizza){
                        $sqlQuery = "INSERT INTO `ordered_article` (`ordering_id`, `article_id`, `status`)
                        VALUES (" . $ordering_id . ', ' . (int)$item['article_id'] . ", 0)";
                        $this->_database->query($sqlQuery);
                        // Adding ordered article to session
                        $ordered_article_id = (int)$this->_database->insert_id;
                        if (!in_array($ordered_article_id, $session_id))
                            array_push($session_id, $ordered_article_id);
                        $_SESSION['ordered_article'] = $session_id;
                    }
                }
            }

            header('Location: Kunde.php');
            exit();
        }
    }

    public static function main():void
    {
        session_start();
        try {
            $page = new Bestellung();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            //header("Content-type: text/plain; charset=UTF-8");
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

Bestellung::main();

