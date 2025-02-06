'use strict'

class ArrayStorage{
    constructor(name){
        this.name = name;
        this.list = this.get();
    }

    get(){
        if(!localStorage.getItem(this.name)){
            localStorage.setItem(this.name, '[]');
        }
        return JSON.parse(localStorage.getItem(this.name));
    }

    set(value){
        this.list.push(value);
        localStorage.setItem(this.name, JSON.stringify(this.list));
    }

    remove(value){
        const index = this.list.indexOf(value);
        this.list.splice(index, 1);
        localStorage.setItem(this.name, JSON.stringify(this.list));
    }

    clear(){
        localStorage.removeItem(this.name);
    }
}