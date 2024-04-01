const todoForm = document.getElementById('todoForm');
const todoInput = document.getElementById('todoInput');
const todoList = document.getElementById('todoList');

const apiUrl = 'http://localhost:8080/todos/'

// Fetch todos from server and display them
async function fetchTodos() {
  try {
    const response = await fetch(`${apiUrl}/todos`);
    const todos = await response.text();
    console.log('Todos:', todos);
    displayTodos(todos.split('\n'));
  } catch (error) {
    console.error('Error fetching todos:', error);
  }
}

// Display todos in the list
function displayTodos(todos) {
  todoList.innerHTML = '';
  todos.forEach(todo => {
    const li = document.createElement('li');
    li.textContent = todo;
    todoList.appendChild(li);
  });
}

// Add new todo
async function addTodo() {
  const title = todoInput.value.trim();
  if (title === '') return;

  try {
    const response = await fetch(`${apiUrl}/todos`, {
      method: 'POST',
      headers: {
        'Content-Type': 'text/plain'
      },
      body: title
    });

    if (!response.ok) {
      throw new Error('Failed to add todo');
    }

    todoInput.value = '';
    fetchTodos();
  } catch (error) {
    console.error('Error adding todo:', error);
  }
}

// Event listener for todo form submission
todoForm.addEventListener('submit', event => {
  event.preventDefault();
  addTodo();
});

// Fetch todos when the page loads
fetchTodos();
