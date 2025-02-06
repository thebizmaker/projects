<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

// to do: change name 'PageTemplate' throughout this file
require_once './Page.php';

class Baecker extends Page
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
        $sqlQuery = "SELECT ordered_article.status AS `status`, ordered_article.ordered_article_id AS ordered_id,
        article.name AS `name`, ordering.ordering_id AS ordering_id, article.picture AS picture FROM ordered_article JOIN article
        ON ordered_article.article_id = article.article_id JOIN ordering ON ordered_article.ordering_id = ordering.ordering_id
        WHERE ordered_article.status < 3 ORDER BY ordering.ordering_id";
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
        if(empty($data)){
            $this->generatePageHeader('Baecker', "", true);
            echo '<section class="section">';
            echo '<h2>' . 'Bäcker' . '</h2>';
            echo '<p>' . 'Hier gibt es nichts zum Backen !' . '</p>';
            echo '</section>';
            $this->generatePageFooter();
            return;
        }
        $this->generatePageHeader('Baecker', "", true); //to do: set optional parameters
        // to do: output view of this page
        echo <<< HTML
            <section class="section">
            <h2>Bäcker</h2>
            
        HTML;
        function sindAlleFertig($alleStatus = []){
            foreach($alleStatus as $status){
                if($status != 2)
                    return false;
            }
            return true;
        }
        
        if(!empty($data)){
            $formId = random_int(1, 1000);
            //echo '<div class="">';
            $old_ordering_id = -1;
            foreach($data as $item){
                $status = (int)$item['status'];
                if($old_ordering_id != $item['ordering_id']){
                    if($old_ordering_id != -1){
                        echo '</div>';
                    }
                    echo '<hr><h3>' . 'Bestellnummer: #' . $item['ordering_id'] . '</h3>';
                    echo '<div class="wraper order">';
                    
                    $old_ordering_id = $item['ordering_id'];
                }
                echo '<div class="item"><form action="Baecker.php" method="post" id="' . $formId . '">';
                echo '<img src="' . htmlspecialchars($item['picture']) . '" alt="' . htmlspecialchars($item['name']) .'">';
                echo '<p>'. htmlspecialchars($item['name']) . '</p>';
                echo '<div class="radio-container">'.'<div>';
                echo '<label>';
                echo '<input type="radio" class="input" name="'.htmlspecialchars($item['ordered_id']).'" value="0" '.($status == 0 ? 'checked ' : '').
                'onclick="document.getElementById('.$formId.').submit()">';
                echo '<span>'.'Bestellt'.'</span>';
                echo '</label>';
                echo '<label>';
                echo '<input type="radio" class="input" name="'.htmlspecialchars($item['ordered_id']).'" value="1" '.($status == 1 ? 'checked ' : '').
                'onclick="document.getElementById('.$formId.').submit()">';
                echo '<span>'.'Im Ofen'.'</span>';
                echo '</label>';
                echo '<label>';
                echo '<input type="radio" class="input" name="'.htmlspecialchars($item['ordered_id']).'" value="2" '.($status == 2 ? 'checked ' : '').
                'onclick="document.getElementById('.$formId.').submit()">';
                echo '<span>Fertig</span>';
                echo '</label>';
                echo '</div>'.'</div>';
                echo '</form></div>';
                $formId++;
            }
            echo '</div>';
            $alleStatus = array();
            foreach($data as $item){
                array_push($alleStatus, $item['status']);
            }
            if(sindAlleFertig($alleStatus)){
                echo '<p>' . 'Alle Bestellungen sind bereit zur Auslieferung !' . '</p>';
            }
        }
        echo '</section>';
        $this->generatePageFooter();
    }

    protected function processReceivedData():void
    {
        parent::processReceivedData();
        // to do: call processReceivedData() for all members
        if($_SERVER['REQUEST_METHOD'] === 'POST' && !empty($_POST)){
            foreach($_POST as $ordered_id => $status){
                $sqlQuery = 'UPDATE `ordered_article` SET `status`='. (int)$status .
                ' WHERE ordered_article_id =' . (int)$ordered_id;
                $this->_database->query($sqlQuery);
            }
            header('Location: Baecker.php');
            exit();
        }
    }

    public static function main():void
    {
        try {
            $page = new Baecker();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            //header("Content-type: text/plain; charset=UTF-8");
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

Baecker::main();

