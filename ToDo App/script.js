'use strict'

const list = document.getElementById('list');
const input = document.getElementById('input');
const add = document.getElementById('add');
const clear = document.getElementById('clear');
const url = document.getElementById('url');
const load = document.getElementById('load');

const storage = new ArrayStorage('tasks');
const tasks = storage.list;

function addTask(task){
    if(typeof task === 'string' && task.length != 0){
        const li = document.createElement('li');
        const remove = document.createElement('button');
        li.textContent = task;
        remove.textContent = "REMOVE";        
        li.appendChild(remove);
        list.append(li);
        remove.onclick = () => {
            li.remove();
            storage.remove(task);
        };       
    }
};

for(let task of tasks)
    addTask(task);

clear.onclick = () => {
    list.innerHTML = '';
    storage.clear();
};
add.onclick = () => {
    let tache = input.value;
    if(storage.list.indexOf(tache) === -1)
    {
        addTask(tache);
        storage.set(tache);
    }
    input.value = "";
};
input.addEventListener('keydown',(evennement) => {
    if(evennement.key === 'Enter')
        add.onclick();
});
url.addEventListener('keydown', (event) => {
    if(event.key === 'Enter')
        load.onclick();
})
load.onclick = () => {
    fetch(url.value)
        .then(res => {
            if(res.ok)
                return res.json()
            throw new Error(`${res.statusText} -> ${res.status}`)
        })
        .then(tasks => {
            console.log('reponse: '+tasks);
            if(Array.isArray(tasks)){
                tasks.forEach(tache => {
                    if(storage.list.indexOf(tache) === -1)
                    {
                        addTask(tache);
                        storage.set(tache);
                    }
                    url.value = "";
                })
                return
            }
            throw TypeError(`La reponse n'est pas un tableau JSON (type: ${typeof tasks})`);
        })
}