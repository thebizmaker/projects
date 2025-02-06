import axios from 'axios';
import React, { useEffect, useState } from 'react';

const Employee = () => {
    document.documentElement.lang = 'de';
    document.getElementsByTagName('body')[0].classList.add('bg-dark');

    const [employees, setEmployees] = useState([]);
    const [showForm, setShowForm] = useState(false);
    const [newEmployee, setNewEmployee] = useState({ firstName: '', lastName: '', email: '', password: '' });
    const [editingEmployee, setEditingEmployee] = useState(null);

    useEffect(() => {
        const fetchData = () => {
            axios.get('http://localhost:8080/employees')
            .then(response => {
                setEmployees(response.data);
            }).catch((error) => {
                console.error('Error:', error);
            });
        }
        fetchData();
        const interval = setInterval(fetchData, 2000);
        return () => clearInterval(interval);
    }, []);

    const handleAddEmployee = () => {
        if(editingEmployee) {
        axios.put(`http://localhost:8080/employee/${editingEmployee.id}`, newEmployee)
            .then(response => {
                setEmployees(employees.map(emp => emp.id === editingEmployee.id ? response.data : emp));
                setNewEmployee({ firstName: '', lastName: '', email: '', password: '' });
                setShowForm(false);
                setEditingEmployee(null);
            }).catch((error) => {
                console.error('Error:', error);
            });
        } else {
            axios.post('http://localhost:8080/employee', newEmployee)
                .then(response => {
                    setEmployees([...employees, response.data]);
                    setNewEmployee({ firstName: '', lastName: '', email: '', password: '' });
                    setShowForm(false);
                }).catch((error) => {
                    console.error('Error:', error);
                });
        }
    };

    const handleEditEmployee = (employee) => {
        setNewEmployee({ firstName: employee.firstName || '', lastName: employee.lastName || '', email: employee.email || '' });
        setEditingEmployee(employee);
        setShowForm(!showForm);
    };

    const handleDeleteEmployee = (id) => {
        axios.delete(`http://localhost:8080/employee/${id}`)
            .then(() => {
                setEmployees(employees.filter(emp => emp.id !== id));
            }).catch((error) => {
                console.error('Error:', error);
            });
    };

    return (
        <div className='container mt-3 bg-dark text-white p-3 text-center'>
            <h1>Liste aller Mitarbeiter</h1>
            <button className='btn btn-success mb-3' onClick={() => setShowForm(!showForm)}>
                Mitarbeiter hinzufügen
            </button>
            {showForm && (
                <div className='card p-3 mb-3 bg-dark' style={{ width: '50%' }}>
                    <div className='form-group'>
                        <input
                            type="text"
                            className='form-control mb-3'
                            placeholder="Vorname"
                            value={newEmployee.firstName}
                            onChange={(e) => setNewEmployee({ ...newEmployee, firstName: e.target.value })}
                        />
                    </div>
                    <div className='form-group'>
                        <input
                            type="text"
                            className='form-control mb-3'
                            placeholder="Nachname"
                            value={newEmployee.lastName}
                            onChange={(e) => setNewEmployee({ ...newEmployee, lastName: e.target.value })}
                        />
                    </div>
                    <div className='form-group'>
                        <input
                            type="email"
                            className='form-control mb-3'
                            placeholder="E-Mail"
                            value={newEmployee.email}
                            onChange={(e) => setNewEmployee({ ...newEmployee, email: e.target.value })}
                        />
                    </div>
                    <button className='btn btn-primary mt-3' onClick={handleAddEmployee}>
                        {editingEmployee ? 'Updaten' : 'Speichern'}
                    </button>
                </div>)}
            <table className="table table-striped table-dark">
                <thead>
                    <tr>
                        <th>ID</th>
                        <th>Vorname</th>
                        <th>Nachname</th>
                        <th>E-Mail</th>
                        <th>Maßnahmen</th>
                    </tr>
                </thead>
                <tbody>
                    {employees.map((employee) => (
                        <tr key={employee.id}>
                            <td>{employee.id}</td>
                            <td>{employee.firstName}</td>
                            <td>{employee.lastName}</td>
                            <td>{employee.email}</td>
                            <td style={{ display: 'flex', gap: '15px' }}>
                                <button className="btn btn-primary" onClick={() => handleEditEmployee(employee)}>Bearbeiten</button>
                                <button className="btn btn-danger" onClick={() => handleDeleteEmployee(employee.id)}>Löschen</button>
                            </td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
};

export default Employee;