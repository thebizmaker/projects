<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

// to do: change name 'PageTemplate' throughout this file
require_once './Page.php';

class Fahrer extends Page
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
        $sqlQuery = "SELECT oa.status AS `status`, a.name AS `name`, oa.ordering_id AS order_id, a.picture AS picture, a.price AS preis, o.address AS `adresse`
                    FROM ordered_article oa JOIN article a ON oa.article_id = a.article_id
                    JOIN ordering o ON oa.ordering_id = o.ordering_id
                    WHERE oa.ordering_id IN (
                        SELECT ordering_id
                        FROM ordered_article
                        GROUP BY ordering_id
                        HAVING COUNT(DISTINCT status) = 1
                            AND MIN(status) IN (2, 3)
                    )
                    ORDER BY oa.ordering_id;";
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
        // check if all orders are delivered
        function alleFertig($alleStatus = []){
            foreach($alleStatus as $status){
                if($status != 4)
                    return false;
            }
            return true;
        }
        // if there are no orders to deliver
        if(empty($data)){
            $this->generatePageHeader('Fahrer',"", true);
            echo '<section class="section">';
            echo '<h2>' . 'Fahrer' . '</h2>';
            echo '<p>' . 'Hier gibt es nichts zum Ausliefern !' . '</p>';
            echo '</section>';
            $this->generatePageFooter();
            return;
        }
        $this->generatePageHeader('Fahrer', "", true); //to do: set optional parameters
        // to do: output view of this page
        echo <<< HTML
            <section class="section">
            <h2>Fahrer</h2>
        HTML;
        $orders = [];
        // group articles by order
        foreach($data as $item){
            $orders[$item['order_id']]['adresse'] = $item['adresse'];
            $orders[$item['order_id']]['status'] = $item['status'];
            $orders[$item['order_id']]['articles'][] = [
                'name' => $item['name'],
                'preis' => $item['preis'],
                'picture' => $item['picture']
            ];
        }
        foreach($orders as $order_id => $order){
            $old_ordering_id = -1;
            $gesamtpreis = 0;
            // display order's details
            foreach($order['articles'] as $article){
                if($old_ordering_id != $order_id){
                    if($old_ordering_id != -1){
                        echo '</div>';
                    }
                    echo '<hr><h3>' . 'Bestellnummer: #' . $order_id . '</h3>';
                    echo '<div class="wraper order">';
                    
                    $old_ordering_id = $order_id;
                }
                echo '<div>';
                echo '<img src="' . htmlspecialchars($article['picture']) . '" alt="' . htmlspecialchars($article['name']) .'">';
                echo '<p>' . htmlspecialchars($article['name']) . '</p>';
                echo '</div>';
                $gesamtpreis += (float)$article['preis'];
            }
            // display address and total price
            echo '</div>';
            echo '<p>' . "Adresse: " . htmlspecialchars($order['adresse']) . '</p>';
            echo '<p>' . 'Gesamtpreis: ' . $gesamtpreis . '€' . '</p>';
            $formId = random_int(1, 1000);
            echo '<form action="Fahrer.php" method="post" id="' . $formId . '">';
            echo '<div class="radio-container">';
            // display radio buttons for changing status
            echo '<div>';
            echo '<label>'.'<input type="radio" name="'.$order_id.'" value="2"
                onclick="document.getElementById('.$formId.').submit()"'. ($order['status'] == 2 ? ' checked' : '').'><span>Fertig</span>'.'</label>';
            echo '<label>'.'<input type="radio" name="'.$order_id.'" value="3"
                onclick="document.getElementById('.$formId.').submit()"'. ($order['status'] == 3 ? ' checked' : '').'><span>Unterwegs</span>'.'</label>';
            echo '<label>'.'<input type="radio" name="'.$order_id.'" value="4"
                onclick="document.getElementById('.$formId.').submit()">'.'<span>Ausgeliefert</span>'.'</label>'.'</div>';
            echo '</div>';
            echo '</form>';
        }
        // mark the order as displayed
        $alleStatus = array();
        foreach($data as $item){
            array_push($alleStatus, $item['status']);
        }
        if(alleFertig($alleStatus)){
            echo '<p>' . 'Alle Bestellungen wurden ausgeliefert !' . '</p>';
        }
        echo '</section>';
        
        $this->generatePageFooter();
    }

    protected function processReceivedData():void
    {
        parent::processReceivedData();
        // to do: call processReceivedData() for all members
        if(!empty($_POST)){
            foreach($_POST as $ordering_id => $status){
                $sqlQuery = 'UPDATE `ordered_article` SET `status`='. (int)$status .
                ' WHERE ordering_id =' . (int)$ordering_id;
                $this->_database->query($sqlQuery);
            }
            header('Location: Fahrer.php');
            exit();
        }
    }

    public static function main():void
    {
        try {
            $page = new Fahrer();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

Fahrer::main();

