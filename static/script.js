document.addEventListener('DOMContentLoaded', () => {
    const bikeState = document.getElementById('bike-state');
    const bikeAlert = document.getElementById('bike-alert');
    const unlockButton = document.getElementById('unlock-button');
    const lockButton = document.getElementById('lock-button');

    // Fetch the current bike status
    const fetchStatus = async () => {
        const response = await fetch('/get_status');
        const data = await response.json();
        bikeState.textContent = data.state;
        bikeAlert.textContent = data.alert ? 'Yes' : 'No';
    };

    // Update bike status
    const updateStatus = async (state, alert = false) => {
        await fetch('/update_status', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ state, alert }),
        });
        fetchStatus();
    };

    // Event listeners
    unlockButton.addEventListener('click', () => updateStatus('Unlocked'));
    lockButton.addEventListener('click', () => updateStatus('Locked'));

    // Initial fetch
    fetchStatus();
});
